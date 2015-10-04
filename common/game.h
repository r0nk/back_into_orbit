#ifndef GAME
#define GAME 1

#include <stdio.h>
#include "poly.h"
#include "model.h"

struct unit {
	double speed;
	point location;
	point destination;
	int health;
};

static inline void dump_unit(struct unit p){
	printf("speed:%f\n",p.speed);
	printf("location:");
	dump_vector(p.location);
	printf("destination:");
	dump_vector(p.destination);
}

#endif
