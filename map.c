#include "map.h"
#include <stdlib.h>
#include <stdio.h>

struct map mkmap()
{
	struct map *m = calloc(1,sizeof(struct map));
	int x,y,z;
	for(x=0;x<10;x++){
		for(z=0;z<10;z++){
			m->tiles[x][0][z]=1;
		}
	}
	for(z=0;z<10;z++){
		for(y=0;y<10;y++){
			m->tiles[0][y][z]=1;
		}
	}
	for(z=0;z<10;z++){
		for(y=0;y<10;y++){
			m->tiles[10][y][z]=1;
		}
	}
	for(x=0;x<10;x++){
		for(y=0;y<10;y++){
			m->tiles[x][y][0]=1;
		}
	}
	for(x=0;x<10;x++){
		for(y=0;y<10;y++){
			m->tiles[x][y][10]=1;
		}
	}
	return *m;
}
