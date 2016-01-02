#include <math.h>

#include "map.h"
#include "room.h"
#include "pathfinding.h"

#define WALKABLE ((world_map.current_room->layout.tiles[i][j])&&(world_map.current_room->layout.tiles[i][j]!='#'))

struct vector parent[MAX_ROOM_WIDTH][MAX_ROOM_HEIGHT];

void init_layouts(struct layout * visited,struct layout * distance, 
		struct vector starting)
{
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			visited->tiles[i][j]=0;
			distance->tiles[i][j]=120;
			parent[i][j]=(struct vector) {0,0,0};
		}
	}
	distance->tiles[(int)starting.x][(int)starting.z]=0;
}

int length(struct vector a , struct vector b)
{
	return (int)sqrt(pow((b.x-a.x),2)+pow((b.z-a.z),2));
}

struct vector min_distance(struct layout *distance, struct layout *visited,
		struct vector goal)
{
	int i,j;
	int min = 120;
	struct vector node;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(!visited->tiles[i][j] && 
					distance->tiles[i][j] <= min &&
					WALKABLE){
				int d = distance->tiles[i][j]+
					length(goal,(struct vector) {i,0,j});
				if(d<=min){
					min=d;
					node = (struct vector) {i,0,j};
				}
			}
		}
	}
	return node;
}

void check_node(int i, int j, struct vector current,
		struct layout * distance, struct layout * visited)
{
	if(visited->tiles[i][j] || !WALKABLE){
		return;
	}
	if(2<=length(current,(struct vector){i,0,j}))
		return;
	int d = distance->tiles[(int)current.x][(int)current.z]+
		length(current,(struct vector) {i,0,j});
	int c = (distance->tiles[(int)current.x][(int)current.z] != 120)
		&&(d < distance->tiles[i][j]);
	if(c){
		distance->tiles[i][j] = d;
		parent[i][j] = current;
	}
}

void visit_next(struct layout * distance,struct layout * visited,
		struct vector goal)
{
	struct vector current = min_distance(distance,visited,goal);

	visited->tiles[(int)current.x][(int)current.z]=1;
	int i = (int)current.x;
	int j = (int)current.z;

	check_node(i,j+1,current,distance,visited);
	check_node(i+1,j,current,distance,visited);
	check_node(i+1,j+1,current,distance,visited);
	check_node(i+1,j-1,current,distance,visited);

	check_node(i,j-1,current,distance,visited);
	check_node(i-1,j,current,distance,visited);
	check_node(i-1,j-1,current,distance,visited);
	check_node(i-1,j+1,current,distance,visited);
}

void path_push(struct path * p, struct vector v)
{
	p->interpoint[p->n_interpoints]=v;
	if(p->n_interpoints >= MAX_INTERPOINTS)
		err(-60,"tried to add over N_INTERPOINTS");
	p->n_interpoints++;
}

struct vector path_pop(struct path * p)
{
	if(p->n_interpoints>0){
		p->n_interpoints--;
		return p->interpoint[p->n_interpoints+1];
	}else{
		return p->destination;
	}
}

/*read the path from the parent data*/
struct path generate_path(struct vector starting, struct vector goal)
{
	struct path p = {0}; 
	p.n_interpoints=0;
	p.destination=goal;
	struct vector n = goal;
	path_push(&p,n);
	while(!(n.x == starting.x && n.z == starting.z)){
		n = parent[(int)n.x][(int)n.z];

		if(n.x == 0 && n.z==0)
			break;

		path_push(&p,n);
	}
	return p;
}

struct path pathfind(struct vector starting, struct vector goal)
{
	struct layout visited,distance;
	struct path path = {0};

	int i,j;
	i=((int)goal.x);j=((int)goal.z);

	if(!WALKABLE || (i<0 || j<0))
		return path;

	init_layouts(&visited,&distance,starting);

	int found=0;//wether or not we found a path
	for(i=0;i<MAX_ROOM_WIDTH*MAX_ROOM_HEIGHT;i++){
		visit_next(&distance,&visited,goal);
		if(visited.tiles[(int)goal.x][(int)goal.z]){
			found=1;
			break;
		}
	}
	if(!found)
		return path;
	path = generate_path(starting,goal);
	return path;
}
