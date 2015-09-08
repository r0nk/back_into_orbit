#ifndef GAME
#define GAME 1
#include "poly.h"

int init_game();

struct player {
	struct model model;
	struct model dest_model;
	point location;
	point destination;
};

extern struct player main_player;

#endif
