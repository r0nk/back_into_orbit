#ifndef GAME
#define GAME 1

#include <stdio.h>
#include "poly.h"
#include "pathfinding.h"
#include "model.h"
#include "inventory.h"

struct bullet {
	double speed;
	point location;
	struct vector direction;
	double duration;
	double damage;
	int flags;
};

struct unit {
	double speed;
	struct vector rotation;
	double rotation_angle;
	point location;
	struct path path;
	double path_timer;
	double health;
	double max_health;
	double cooldown;
	double cooldown2;/* moles teleportation cooldown */
	int connected_to; /* index of other yo */
	double resist;
	double damage;
	struct inventory inventory;
	int type;
#define UNIT_TYPE_PLAYER 		1
#define UNIT_TYPE_SHOP   		2
#define UNIT_TYPE_ITEM   		3
#define UNIT_TYPE_NEUTRAL_CREEP   	4
#define UNIT_TYPE_RANGER	   	5
#define UNIT_TYPE_BOSS   		6
#define UNIT_TYPE_MOLE   		7
#define UNIT_TYPE_YO   			8
	int team;
#define NEUTRAL_TEAM 			0
#define RED_TEAM 			1
#define BLUE_TEAM 			2
	int flags;
#define HAS_TRIGGER			1
#define HAS_VAIL			2
#define HAS_VECTOR_FIELD		4
	double poison_timer;
	double dash_timer;
	char * saying;
	double hit_radius;
	int score;
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
	printf("\nhealth:%f\n",p.health);
}

struct unit scavenger_npc(struct vector location);
struct unit ranger_npc(struct vector location);
struct unit item_npc(struct vector location,int item_id);

#endif
