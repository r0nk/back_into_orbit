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

static inline struct layout get_layout(char * pathname)
{
	int fd = open(pathname,O_RDONLY);
	if(fd==-1)
		err(-29,"get_layout() couldn't open");

	struct layout *m = calloc(1,sizeof(struct layout));
	char c[2];
	int x=0,y=0;

	while(read(fd,c,1)){
		switch(c[0]){
		case '\n':
			y++;
			x=0;
			continue;
		case '.':
		case ' ':
			m->tiles[x][y]='\0';
			break;
		default:
			m->tiles[x][y]=c[0];
			break;
		}
		x++;
	}
	return *m;
}

struct room generate_room();

#endif
