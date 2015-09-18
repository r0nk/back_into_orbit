#ifndef GAME
#define GAME 1

#include "poly.h"
#include "model.h"
#include "game_state.h"

struct player {
	struct model model;
	struct model dest_model;
	double speed;
	point location;
	point destination;
};

struct game_state init_game();

#endif
