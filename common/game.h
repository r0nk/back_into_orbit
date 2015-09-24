#ifndef GAME
#define GAME 1

#include <stdio.h>
#include "poly.h"
#include "model.h"

struct player {
	double speed;
	point location;
	point destination;
};

static inline void dump_player(struct player p){
	printf("speed:%f\n",p.speed);
	printf("location:");
	dump_vector(p.location);
	printf("destination:");
	dump_vector(p.destination);
}


#endif
