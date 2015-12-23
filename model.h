#ifndef MODEL
#define MODEL
#include "poly.h"

struct model pawn();
struct model floor_tile(struct vector d,struct vector l);
struct model bullet();
struct model player_model();
struct model wall_block(struct vector c,struct vector d,struct vector l);
struct model flag_holder_model();
struct model gold_coin_model();
struct model shopkeeper_model();
struct model doorway_model();
struct model scavenger();
struct model portal_model();
struct model pedestal_model();
struct model puzzle_model();
struct model regen_model();
struct model dice_model();
struct model trigger_model();
struct model vail_model();
struct model dash_model();
struct model vector_field_model();
struct model shield_model();
struct model kite_model();
struct model capacitor_model();
struct model boss_model();

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

void add_submodel(struct model * to, struct model * from);
 
#endif
