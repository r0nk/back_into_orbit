#ifndef GAME
#define GAME 1
#include "poly.h"

int init_game();

struct player {
	struct model model;
	point location;
};

extern struct player main_player;

#endif
