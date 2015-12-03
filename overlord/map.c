#include "map.h"

struct map init_map()
{
	struct map map;
	int i;
	for(i=0;i<MAX_EDGES;i++)
		map.edge[i]= (struct edge) {0};

	map.edge[0].a_index=0;
	map.edge[0].b_index=1;
	map.edge[0].a_door=1;
	map.edge[0].b_door=2;
	return map;
}
