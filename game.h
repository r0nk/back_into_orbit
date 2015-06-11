#ifndef GAME
#define GAME 1
#include "poly.h"

int init_game();

struct player {
	struct model model;
	struct point location;
};

extern struct player p;

#endif
