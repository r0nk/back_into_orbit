#ifndef ROOM 
#define ROOM

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#include "game_state.h"

#define MAX_DOORWAYS 6

struct layout {
	char tiles[100][100];
};

struct doorway {
	int index;
	int x,z;
	int is_connected;
};

struct room {
	struct layout layout;
	int n_doorways;
	struct doorway doorway[MAX_DOORWAYS];
	struct game_state gs;
};

extern struct model block;

#define LAYOUT_WALL '#'

struct room generate_room();

#endif
