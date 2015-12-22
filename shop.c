#include "shop.h"
#include "room.h"
#include "inventory.h"


struct transaction generate_transaction(int x, int z)
{
	struct transaction transaction;
	transaction.item=pick_item();
	transaction.price=3+(rand()%3);
	transaction.location = (struct vector) {x,0,z};
	return transaction;
}

struct shop generate_shop(struct room * room)
{
	struct shop shop;
	int i=0;
	int x,z;
	for(z=0;z<MAX_ROOM_HEIGHT;z++){
		for(x=0;x<MAX_ROOM_WIDTH;x++){
			if(room->layout.tiles[x][z]=='K'){
				shop.keeper_location=(struct vector){x,0,z};
			}
			if(room->layout.tiles[x][z]=='i'){
				shop.t[i]=generate_transaction(x,z);
			}
		}
	}
	return shop;
}

