#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

struct map mkmap(char * pathname)
{
	int fd = open(pathname,O_RDONLY);
	struct map *m = calloc(1,sizeof(struct map));
	char c[2];
	int x=0,y=0;

	while(read(fd,c,1)){
		switch(c[0]){
		case '\n':
			y++;
			x=0;
			continue;
		case '#':
			m->tiles[x][y]=1;
			break;
		default:
			break;
		}
		x++;
	}
	return *m;
}
