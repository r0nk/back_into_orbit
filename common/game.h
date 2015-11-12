#ifndef GAME
#define GAME 1

#include <stdio.h>
#include "poly.h"
#include "model.h"
#include <inventory.h>

struct bullet {
	double speed;
	point location;
	struct vector direction;
	double duration;
};

struct unit {
	double speed;
	point location;
	point destination;
	double health;
	double cooldown;
	struct inventory inventory;
	int team;
#define RED_TEAM 1
#define BLUE_TEAM 2
};

static inline void dump_bullet(struct bullet b){
	printf("speed:%f\n",b.speed);
	printf("location:");
	dump_vector(b.location);
	printf("\n direction:");
	dump_vector(b.direction);
}

static inline void dump_unit(struct unit p){
	printf("speed:%f\n",p.speed);
	printf("location:");
	dump_vector(p.location);
	printf("\ndestination:");
	dump_vector(p.destination);
	printf("\nhealth:%f\n",p.health);
	printf("\nteam:%i\n",p.team);
}

#endif
