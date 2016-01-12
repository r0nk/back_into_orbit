#include <stdlib.h>
#include "map.h"

int walkable(int i, int j)
{
	struct layout * l = &world_map.current_room->layout;
	if(!l->tiles[i][j])
		return 0;
	if(l->tiles[i][j]=='#') return 0;
	return 1;
}

struct doorway * get_doorway_by_index(struct map * map, int index)
{
	int i,j;
	if(index==-1)
		return NULL;
	for(i=0;i<map->n_rooms;i++){
		for(j=0;j<(map->room[i].n_doorways);j++){
			if(map->room[i].doorway[j].index == index){
				//printf("dbi(%i): (r:%i,d[%i])\n",index,i,j);
				return &map->room[i].doorway[j];
			}
		}
	}
	fprintf(stderr,"ERR: get_doorway_by_index: door not found\n");
	return NULL;
}

struct room * get_room_by_doorway_index(struct map * map, int index)
{
	int i,j;
	for(i=0;i<map->n_rooms;i++){
		for(j=0;j<(map->room[i].n_doorways);j++){
			if(map->room[i].doorway[j].index == index){
				return &map->room[i];
			}
		}
	}
	fprintf(stderr,"ERR: get_room_by_doorway_index: door not found\n");
	return NULL;
}

struct doorway * get_nonconnected_door(struct map * map)
{
	int i;
	struct doorway * doorway;
	int rando=rand();

	for(i=0;i<(map->n_doorways);i++){
		rando=rand();
		doorway = get_doorway_by_index(map,rando%(map->n_doorways));
		if(doorway==NULL)
			continue;

		if(!doorway->is_connected)
			return doorway;
	}

	/* fine rando you selfish bastard, we'll just brute force it then */
	for(i=0;i<(map->n_doorways);i++){
		doorway = get_doorway_by_index(map,i);
		if(doorway==NULL)
			continue;

		if(!doorway->is_connected)
			return doorway;
	}

	fprintf(stderr,"ERR: next_nonconnected_door: no nonconnected door\n");
	exit(10);
	return NULL;
}

void connect_edges(struct map * map,struct doorway * d1, struct doorway * d2)
{
	if(map->n_edges >= MAX_EDGES)
	{
		printf("WARNING: tried to generate over max edges\n");
		return;
	}
	struct room *room;

	d1->is_connected=1;
	d2->is_connected=1;
	map->edge[map->n_edges].a=d1->index;
	map->edge[map->n_edges].b=d2->index;

	room = get_room_by_doorway_index(map,d2->index);
	d1->color=room->color;
	room = get_room_by_doorway_index(map,d1->index);
	d2->color=room->color;

	map->n_edges++;
}

void generate_edge(struct map * map)
{
	struct doorway * d1, * d2;
	struct room *room;

	d1 = get_nonconnected_door(map);
	d2 = get_nonconnected_door(map);

	if(d1==d2) //prevent self-doors
		d1 = get_nonconnected_door(map);
	if(d1==d2) //prevent self-doors
		d1 = get_nonconnected_door(map);
	if(d1==d2) //prevent self-doors
		d1 = get_nonconnected_door(map);

	room = get_room_by_doorway_index(map,d1->index);
	if(room==get_room_by_doorway_index(map,d2->index))
		d2 = get_nonconnected_door(map);


	connect_edges(map,d1,d2);
}

/*make sure the player starting room, the shop, and the boss are all connected 
 * first */
void generate_edges_for_specials(struct map * map)
{
	connect_edges(map,&map->room[0].doorway[0],get_nonconnected_door(map));
	connect_edges(map,&map->room[8].doorway[0],get_nonconnected_door(map));
	connect_edges(map,&map->room[9].doorway[0],get_nonconnected_door(map));
}

void generate_edges(struct map * map)
{
	int i;
	struct doorway * d;
	map->n_edges=0;
	/* we reset all the connections first */
	for(i=0;i<(map->n_doorways);i++){
		d = get_doorway_by_index(map,i);
		d->is_connected=0;
	}
	generate_edges_for_specials(map);
	for(i=0;i<((map->n_doorways/2)-3);i++){
		generate_edge(map);
	}
}

void index_doorways(struct map * map)
{
	int i,j,index = 0;
	for(i=0;i<map->n_rooms;i++){
		for(j=0;j<(map->room[i].n_doorways);j++){
			map->room[i].doorway[j].index=index;
			map->room[i].doorway[j].color=(struct vector){0,255,0};
			index++;
		}
	}
	map->n_doorways=index;
}

void generate_rooms(struct map * map)
{
	int i;
	for(i=0;i<MAX_ROOMS;i++){
		map->room[i]=generate_room(i,map->level);
	}
	map->n_rooms=MAX_ROOMS;
	index_doorways(map);
}

void generate_map(struct map * map,int level)
{
	map->level=level;
	generate_rooms(map);
	generate_edges(map);
	map->current_room=&map->room[0];
}

int other_edge(struct map * map,int index){
	int i;
	for(i=0;i<map->n_edges;i++){
		if(map->edge[i].a==index){
			return map->edge[i].b;
		}
		if(map->edge[i].b==index){
			return map->edge[i].a;
		}
	}
	printf("other edge not found\n");
	return -1;
}

struct doorway * connected_doorway(struct map * map,int i)
{
	return get_doorway_by_index(map,other_edge(map,i));
}

void transfer_rooms(struct map * map, struct room * dest)
{
	dest->gs.game_player = map->current_room->gs.game_player;
	map->current_room=dest;
}

void move_through_doorway(struct map * map,int t)
{
	struct doorway * dest_door;
	struct room * dest_room;
	dest_door = connected_doorway(map,t);
	dest_room = get_room_by_doorway_index(map,dest_door->index);

	if(dest_room!=map->current_room)
		transfer_rooms(map,dest_room);

	bzewerwwww(t,dest_door->index);

	map->current_room->gs.game_player.location = dest_door->location;
	map->current_room->gs.game_player.location.x += 2;

}

struct unit buffer_player;

void transfer_map(int level)
{
	buffer_player = world_map.current_room->gs.game_player;
	generate_map(&world_map,level);
	world_map.current_room->gs.game_player = buffer_player;
	world_map.current_room->gs.game_player.location=(struct vector){5,0,5};
}
