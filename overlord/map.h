#ifndef MAP
#define MAP

#include <stdio.h>
#include <igraph/igraph.h>

#define MAX_EDGES 400

static inline void dump_map(igraph_t m)
{
	igraph_write_graph_edgelist(&m,stdout);
}

igraph_t init_map();

igraph_t overmap;

#endif
