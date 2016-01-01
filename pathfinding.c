#include "map.h"
#include "pathfinding.h"

void init_layouts(struct layout * visited,struct layout * distance, 
		struct vector starting)
{
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			visited->tiles[i][j]=0;
			distance->tiles[i][j]=255;
		}
	}
	visited->tiles[(int)starting.x][(int)starting.z]=1;
}

struct path pathfind(struct vector starting, struct vector goal)
{
	struct layout visited,distance;
	struct vector current=starting;
	init_layouts(&visited,&distance,starting);
	while(!(visited.tiles[(int)goal.x][(int)goal.z])){
//		visit_neighbors(current,&distance,&visited);
		;
	}
}
