#include "map.h"
#include <err.h>
#include <stdio.h>

igraph_t init_map()
{
	igraph_t ret;
	FILE * f = fopen("map.edgelist","r");
	if(f==NULL)
		err(-11,"cant open map.edgelist");
	igraph_read_graph_edgelist(&ret,fopen("map.edgelist","r"),0,0);
	return ret;
}
