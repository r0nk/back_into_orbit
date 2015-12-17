#ifndef EFFECTS
#define EFFECTS
#include "game_state.h"

/*all the item effects */

void item_effect(struct game_state * gs, struct item item,double delta);

void regen_effect(struct game_state * gs, double delta);

#endif
