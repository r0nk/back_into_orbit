#ifndef PATHFINDING
#define PATHFINDING

#include "poly.h"

#define MAX_INTERPOINTS 30
struct path
{
	struct vector destination;/* final desination */
	struct vector interpoints[MAX_INTERPOINTS];
};

struct path pathfind(struct vector starting,struct vector goal);
 
#endif
