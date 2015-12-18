#ifndef INVENTORY
#define INVENTORY

#include <stdlib.h>
#include <stdio.h>

/* units are in the game world, items are in inventories */

struct item {
	int type;
	int active; /* whether or not the player activated this item */
	int stackable;
	int passive; /*if set, it ignores int active entirely*/
	int amount;
};

#define ITEM_REGEN 1
#define ITEM_PUZZLE 2
#define ITEM_DICE 3
#define ITEM_TRIGGER 4
#define ITEM_VAIL 5
#define ITEM_VECTOR_FEILD 6
#define ITEM_ENTROPY_BATTERY 7
#define ITEM_SHEILD 8
#define ITEM_KITE 9
#define ITEM_REMOTE 10
#define ITEM_CAPACITOR 11
#define ITEM_BEACON 12
#define ITEM_BOX 13
#define ITEM_COIN 14
#define ITEM_ACCELERATOR 15
#define ITEM_DASH 16

#define MAX_INVENTORY_SPACE 4

struct inventory {
	int n_items;
	struct item item[MAX_INVENTORY_SPACE];
};

inline static struct item regen_item()
{
	struct item item;
	item.type = ITEM_REGEN;
	item.passive = 1;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item coin_item()
{
	struct item coin;
	coin.stackable = 1;
	coin.amount = 1;
	return coin;
}

inline static char * get_item_name(struct item item)
{
	switch(item.type){
		case ITEM_REGEN:
			return "regen kit";
			break;
		default:
			return "NAME_NOT_FOUND";
			break;
	}
}

 
#endif
