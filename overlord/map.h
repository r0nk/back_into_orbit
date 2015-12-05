#ifndef MAP
#define MAP

#include <stdio.h>

struct node
{
	int index;
	int door;
};

struct edge
{
	struct node a,b;
};

#define MAX_EDGES 400

struct map
{
	int n_edges;
	struct edge edge[MAX_EDGES];
};

static inline void dump_node(struct node n)
{
	printf(" index:%i door:%i\n",n.index,n.door);
}

static inline void dump_edge(struct edge e)
{
	dump_node(e.a);
	dump_node(e.b);
}

static inline void dump_map(struct map m)
{
	int i;
	for(i=0;i<m.n_edges;i++){
		printf("edge[%i]:\n",i);
		dump_edge(m.edge[i]);
	}
}

struct map init_map();

struct map overmap;

struct node connected_node(struct node n);
	
#endif
