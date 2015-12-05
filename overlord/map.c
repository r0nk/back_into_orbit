#include "map.h"
#include <stdio.h>

struct map init_map()
{
	struct map map;
	int i;
	for(i=0;i<MAX_EDGES;i++)
		map.edge[i]= (struct edge) {0};

	map.edge[0].a.index=0;
	map.edge[0].b.index=1;
	map.edge[0].a.door=1;
	map.edge[0].b.door=2;
	map.n_edges=1;
	return map;
}

struct node connected_node(struct node n)
{
	printf("TODO connected_node");
	dump_node(n);
}
