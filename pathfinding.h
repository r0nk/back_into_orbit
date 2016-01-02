#ifndef PATHFINDING
#define PATHFINDING

#include "poly.h"

#define MAX_INTERPOINTS 100
struct path
{
	int n_interpoints;
	struct vector destination;/* final desination */
	struct vector interpoint[MAX_INTERPOINTS];
};

static inline void dump_path(struct path p)
{
	printf("path n_interpoints: %i\n",p.n_interpoints);
	printf("destination: ");
	dump_vector(p.destination);
	printf("\n");
	int i;
	for(i=0;i<p.n_interpoints;i++){
		printf("[%i]:",i);
		dump_vector(p.interpoint[i]);
		printf("\n");
	}
}

struct path pathfind(struct vector starting,struct vector goal);
struct vector path_pop(struct path * p);
 
#endif
