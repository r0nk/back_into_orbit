#ifndef MAP
#define MAP

#include "room.h"

#define MAX_ROOMS 10
#define MAX_EDGES 20

/* basically the portal struct */
struct edge {
	int a;
	int b;
};

struct map {
	struct room * current_room;
	int n_rooms;
	struct room room[MAX_ROOMS];
	int n_edges;
	struct edge edge[MAX_EDGES];
};

struct map generate_map();

void move_through_doorway(struct map * map,int t);

extern struct map world_map;
 
#endif
