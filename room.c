#include "room.h"

int count_doorways(struct room * room)
{
	int ret = 0;
	int x,z;
	for(x=0;x<100;x++){
		for(z=0;z<100;z++){
			if(room->layout.tiles[x][z]>'0' 
					&& room->layout.tiles[x][z]<'9'){
				ret++;
			}
		}
	}
	return ret;
}

struct room generate_room()
{
	struct room room;
	room.layout=get_layout("layouts/simple.layout");
	room.n_doorways=count_doorways(&room);
	return room;
}
