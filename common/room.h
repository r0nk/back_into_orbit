#ifndef ROOM 
#define ROOM
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

struct room {
	char tiles[100][100];
};

extern struct room world_room;
extern struct model block;

#define ROOM_WALL 1
#define ROOM_DOOR 2

static inline struct room mkroom(char * pathname)
{
	int fd = open(pathname,O_RDONLY);
	struct room *m = calloc(1,sizeof(struct room));
	char c[2];
	int x=0,y=0;

	while(read(fd,c,1)){
		switch(c[0]){
		case '\n':
			y++;
			x=0;
			continue;
		case '#':
			m->tiles[x][y]=ROOM_WALL;
			break;
		case '@':
			m->tiles[x][y]=ROOM_DOOR;
			break;
		default:
			break;
		}
		x++;
	}
	return *m;
}

#endif
