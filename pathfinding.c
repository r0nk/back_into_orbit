#include <math.h>

#include "map.h"
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
	visited->tiles[(int)starting.x][(int)starting.z]=1;
}

struct vector min_distance(struct layout *distance, struct layout *visited)
{
	int i,j;
	int min = 120;
	struct vector node;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(!visited->tiles[i][j] && 
					distance->tiles[i][j] <= min &&
					WALKABLE){
				min=distance->tiles[i][j];
				node = (struct vector) {i,0,j};
			}
		}
	}
	return node;
}

int length(struct vector a , struct vector b)
{
	return (int)sqrt(pow((b.x-a.x),2)+pow((b.z-a.z),2));
}

void visit_next(struct layout * distance,struct layout * visited)
{
	struct vector current = min_distance(distance,visited);
	visited->tiles[(int)current.x][(int)current.z]=1;
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(visited->tiles[i][j] && !WALKABLE){
				continue;
			}
			int d = distance->tiles[(int)current.x][(int)current.z]+
				length(current,(struct vector) {i,0,j});
			/*TODO add wallcheck to c*/
			int c = (distance->tiles[(int)current.x][(int)current.z] != 120)
				&&(d < distance->tiles[i][j]);
			if(c){
				distance->tiles[i][j] = d;
				parent[i][j] = current;
			}
		}
	}
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
	struct path p; 
	p.n_interpoints=0;
	p.destination=goal;
	struct vector n = goal;
	path_push(&p,n);
	while(n.x != starting.x && n.z != goal.z){
		n = parent[(int)n.x][(int)n.z];
		path_push(&p,n);
	}
	return p;
}

struct path pathfind(struct vector starting, struct vector goal)
{
	struct layout visited,distance;
	struct path path;

	init_layouts(&visited,&distance,starting);

	while(!(visited.tiles[(int)goal.x][(int)goal.z])){
		visit_next(&distance,&visited);
	}
	path = generate_path(starting,goal);
	return path;
}
