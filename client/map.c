#include "map.h"
#include <stdlib.h>
#include <stdio.h>

struct map mkmap()
{
	struct map *m = calloc(1,sizeof(struct map));
	int x,y;

#define MS 15

	for(y=0;y<MS;y++)
		m->tiles[0][y]=1;
	for(x=0;x<MS;x++)
		m->tiles[x][0]=1;
	for(y=0;y<MS;y++)
		m->tiles[MS][y]=1;
	for(x=0;x<MS;x++)
		m->tiles[x][MS]=1;
	return *m;
}
