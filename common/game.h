#ifndef GAME
#define GAME 1

#include <stdio.h>
#include "poly.h"
#include "model.h"

struct bullet {
	double speed;
	point location;
	struct vector direction;
	double duration;
};

static inline void dump_bullet(struct bullet b){
	printf("speed:%f\n",b.speed);
	printf("location:");
	dump_vector(b.location);
	printf("\n direction:");
	dump_vector(b.direction);
}

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
