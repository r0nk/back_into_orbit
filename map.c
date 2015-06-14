#include "map.h"
#include <stdlib.h>
#include <stdio.h>

struct map mkmap()
{
	struct map *m = calloc(1,sizeof(struct map));
	int x,z;
	printf("mkmap %i\n",sizeof(struct map));
	for(x=0;x<10;x++){
		for(z=0;z<10;z++){
			m->tiles[x][0][z]=1;
		}
	}
	return *m;
}
