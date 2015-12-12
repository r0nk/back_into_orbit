#include <err.h>
#include "map.h"

struct doorway * get_doorway_by_index(struct map * map, int index)
{
	int i,j;
	for(i=0;i<map->n_rooms;i++){
		for(j=0;j<(map->room[i].n_doorways);j++){
			if(map->room[i].doorway[j].index == index)
				return &map->room[i].doorway[j];
		}
	}
	fprintf(stderr,"ERR: get_doorway_by_index: door not found\n");
	return NULL;
}

struct doorway * next_nonconnected_door(struct map * map)
{
	int i;
	struct doorway * doorway;
	for(i=0;i<MAX_EDGES;i++){
		doorway = get_doorway_by_index(map,i);
		if(doorway==NULL)
			continue;

		if(!doorway->is_connected)
			return doorway;
	}
	fprintf(stderr,"ERR: next_nonconnected_door: no nonconnected door\n");
	err(-203,"no nonconnected door");
	return NULL;
}

void generate_edge(struct map * map)
{
	struct doorway * doorway;
	if(map->n_edges >= MAX_EDGES)
	{
		printf("WARNING: tried to generate over max edges\n");
		return;
	}

	doorway = next_nonconnected_door(map);
	doorway->is_connected=1;
	map->edge[map->n_edges].a=doorway->index;

	doorway = next_nonconnected_door(map);
	doorway->is_connected=1;
	map->edge[map->n_edges].b=doorway->index;

	map->n_edges++;
}

void generate_edges(struct map * map)
{
	int i;
	for(i=0;i<MAX_EDGES;i++){
		generate_edge(map);
	}
}

void index_doorways(struct map * map)
{
	int i,j,index = 0;
	for(i=0;i<map->n_rooms;i++){
		for(j=0;j<(map->room[i].n_doorways);j++){
			map->room[i].doorway[j].index=index;
			index++;
		}
	}
}

void generate_rooms(struct map * map)
{
	int i;
	for(i=0;i<MAX_ROOMS;i++){
		map->room[i]=generate_room();
	}
	map->n_rooms=MAX_ROOMS;
	index_doorways(map);
}

struct map generate_map()
{
	struct map map;
	generate_rooms(&map);
	generate_edges(&map);
	return map;
}
