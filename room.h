#ifndef ROOM 
#define ROOM

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#include "game_state.h"

#define MAX_ROOM_WIDTH 100
#define MAX_ROOM_HEIGHT 100

#define MAX_DOORWAYS 6

struct layout {
	char tiles[MAX_ROOM_WIDTH][MAX_ROOM_HEIGHT];
};

struct doorway {
	int index;
	struct vector location;
	int is_connected;
	struct vector color;
};

static inline void dump_doorway(struct doorway * doorway)
{
	printf("doorway->index:%i\n",doorway->index);
	printf("doorway->location:(%f,%f,%f)\n",doorway->location.x,
			doorway->location.y,doorway->location.z);
	printf("doorway->is_connected:%i\n",doorway->is_connected);
}

struct room {
	struct layout layout;
	int n_doorways;
	struct doorway doorway[MAX_DOORWAYS];
	struct game_state gs;
	struct vector color;
	struct model model;
	int shop;
};

static inline void dump_room(struct room * room)
{
	printf("room->layout: TODO\n");
	printf("room->n_doorways: %i\n",room->n_doorways);
	int i;
	for(i=0;i<room->n_doorways;i++){
		printf("room->doorway[%i]:\n",i);
		dump_doorway(&(room->doorway[i]));
	}
	printf("room->gs: TODO\n");
}

extern struct model block;

#define LAYOUT_WALL '#'

struct room generate_room();
void get_layout(struct room * room,char * pathname);

#endif
