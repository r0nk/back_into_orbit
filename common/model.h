#ifndef MODEL
#define MODEL
#include "poly.h"

struct model pawn();
struct model bullet();
struct model player_model();
struct model cube();
struct model tetra();
struct model wall_block();
struct model skull_monkey();
struct model red_pawn_model();
struct model blue_pawn_model();
struct model red_flag_model();
struct model blue_flag_model();
struct model flag_holder_model();
struct model gold_coin();

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
