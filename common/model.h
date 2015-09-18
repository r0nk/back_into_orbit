#ifndef MODEL
#define MODEL
#include "poly.h"

struct model player_model();
struct model cube();
struct model tetra();

struct vertice { 
	struct vector p;/*position*/
	struct vector n;/*normal*/
	struct vector c;/*color*/
};

struct polygon{
	struct vertice v[3];
};

struct model{
	unsigned int cardinality; 
	struct polygon *poly;
};

 
#endif