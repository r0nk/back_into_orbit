#ifndef ROOM 
#define ROOM
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

struct room {
	char tiles[100][100];
};

extern struct room world_room;
extern struct model block;

#define ROOM_WALL '#'
#define ROOM_DOOR 2

static inline struct room mkroom(char * pathname)
{
	int fd = open(pathname,O_RDONLY);
	if(fd==-1)
		err(-29,"mkroom() couldn't open");

	struct room *m = calloc(1,sizeof(struct room));
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

#endif
