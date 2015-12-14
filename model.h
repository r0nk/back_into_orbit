#ifndef MODEL
#define MODEL
#include "poly.h"

struct model pawn();
struct model bullet();
struct model player_model();
struct model wall_block();
struct model flag_holder_model();
struct model gold_coin_model();
struct model shop_model();
struct model doorway_model();
struct model scavenger();
struct model portal_model();

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
