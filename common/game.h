#ifndef GAME
#define GAME 1

#include "poly.h"
#include "model.h"

struct player {
	struct model model;
	struct model dest_model;
	double speed;
	point location;
	point destination;
};

#endif
