#include <string.h>
#include <stdlib.h>
#include "layouts.h"
#include "room.h"

void get_layout(struct room * room,char * pathname)
{
	int fd = open(pathname,O_RDONLY);
	if(fd==-1)
		err(-29,"get_layout() couldn't open");

	char c[2];
	int x=0,y=0;

	memset(&room->layout,0,sizeof(struct layout));

	while(read(fd,c,1)){
		switch(c[0]){
		case '\n':
			y++;
			x=0;
			continue;
		case ' ':
		case '.':
			room->layout.tiles[x][y]='\0';
			break;
		default:
			room->layout.tiles[x][y]=c[0];
			break;
		}
		x++;
	}
}

void count_doorways(struct room * room)
{
	int n = 0;
	int x,z;
	for(z=0;z<100;z++){
		for(x=0;x<100;x++){
			if((room->layout.tiles[x][z]) > ('0'-1)
					&& room->layout.tiles[x][z]<'9'){
				room->doorway[n].x=x;
				room->doorway[n].z=z;
				room->doorway[n].is_connected=0;
				n++;
			}
		}
	}
	room->n_doorways=n;
}

struct room generate_room()
{
	struct room room;
	pick_layout(&room);
	count_doorways(&room);
	room.gs = init_game();
	return room;
}
