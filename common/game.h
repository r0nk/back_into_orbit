#ifndef GAME
#define GAME 1

#include <stdio.h>
#include "poly.h"
#include "model.h"

struct bullet {
	double speed;
	point location;
	struct vector direction;
};

struct unit {
	double speed;
	point location;
	point destination;
	double health;
};

static inline void dump_unit(struct unit p){
	printf("speed:%f\n",p.speed);
	printf("location:");
	dump_vector(p.location);
	printf("\ndestination:");
	dump_vector(p.destination);
	printf("\nhealth:%f\n",p.health);
}

#endif
