#include "room.h"

void count_doorways(struct room * room)
{
	int n = 0;
	int x,z;
	for(x=0;x<100;x++){
		for(z=0;z<100;z++){
			if(room->layout.tiles[x][z]>'0' 
					&& room->layout.tiles[x][z]<'9'){
				n++;
				room->doorway[n].x=x;
				room->doorway[n].z=z;
			}
		}
	}
	room->n_doorways=n;
}

struct room generate_room()
{
	struct room room;
	room.layout=get_layout("layouts/simple.layout");
	count_doorways(&room);
	return room;
}
