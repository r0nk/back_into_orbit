#ifndef INVENTORY
#define INVENTORY

#include <stdlib.h>
#include <stdio.h>

/* units are in the game world, items are in inventories */

struct item {
	int type;
	double cooldown;
	double duration;
	int active; /* whether or not the player activated this item */
	int stackable;
	int passive; /*if set, it ignores int active entirely*/
	int amount;
};

#define ITEM_REGEN 1
#define ITEM_PUZZLE 2
#define ITEM_TELEDICE 3
#define ITEM_TRIGGER 4
#define ITEM_VAIL 5
#define ITEM_VECTOR_FIELD 6
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

inline static struct item capacitor_item()
{
	struct item item;
	item.type = ITEM_CAPACITOR;
	item.cooldown = 0;
	item.passive = 1;
	item.active = 0;
	item.stackable = 1;
	item.amount = 1;
	return item;
}

inline static struct item coin_item()
{
	struct item item;
	item.type = ITEM_COIN;
	item.cooldown = 0;
	item.passive = 1;
	item.active = 0;
	item.stackable = 1;
	item.amount = 1;
	return item;
}

inline static struct item kite_item()
{
	struct item item;
	item.type = ITEM_KITE;
	item.cooldown = 0;
	item.passive = 1;
	item.active = 0;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item dash_item()
{
	struct item item;
	item.type = ITEM_DASH;
	item.cooldown = 3;
	item.passive = 0;
	item.active = 0;
	item.stackable = 0;
	item.amount = 1;
	return item;
}
inline static struct item sheild_item()
{
	struct item item;
	item.type = ITEM_SHEILD;
	item.cooldown = 1;
	item.passive = 0;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item vector_field_item()
{
	struct item item;
	item.type = ITEM_VECTOR_FIELD;
	item.cooldown = 0;
	item.passive = 1;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item vail_item()
{
	struct item item;
	item.type = ITEM_VAIL;
	item.cooldown = 0;
	item.passive = 1;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item trigger_item()
{
	struct item item;
	item.type = ITEM_TRIGGER;
	item.cooldown = 0;
	item.passive = 1;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item regen_item()
{
	struct item item;
	item.type = ITEM_REGEN;
	item.cooldown = 0;
	item.passive = 1;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item teledice_item()
{
	struct item item;
	item.type = ITEM_TELEDICE;
	item.cooldown = 2;
	item.active = 0;
	item.passive = 0;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item puzzle_item()
{
	struct item item;
	item.type = ITEM_PUZZLE;
	item.cooldown = 2;
	item.active = 0;
	item.passive = 0;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static char * get_item_name(struct item item)
{
	switch(item.type){
		case ITEM_REGEN:
			return "regen kit";
		case ITEM_PUZZLE:
			return "puzzle cube";
		case ITEM_TELEDICE:
			return "teledice";
		case ITEM_TRIGGER:
			return "hair trigger";
		case ITEM_VAIL:
			return "vail";
		case ITEM_VECTOR_FIELD:
			return "vector feild";
		case ITEM_ENTROPY_BATTERY:
			return "entropy battery";
		case ITEM_SHEILD:
			return "sheild";
		case ITEM_KITE:
			return "kite";
		case ITEM_REMOTE:
			return "remote";
		case ITEM_CAPACITOR:
			return "capacitor";
		case ITEM_BEACON:
			return "beacon";
		case ITEM_BOX:
			return "black box";
		case ITEM_COIN:
			return "coin";
		case ITEM_ACCELERATOR:
			return "accelerator";
		case ITEM_DASH:
			return "dash circuit";
		default:
			return "NAME_NOT_FOUND";
			break;
	}
}

inline static void add_item(struct inventory * inven, struct item item)
{
	int i;
	if(inven->n_items >= MAX_INVENTORY_SPACE)
	{
		printf("ERR: too many items in inven to add another\n");
	}
	if(item.stackable){
		for(i=0;i<inven->n_items;i++){
			if(item.type == inven->item[i].type){
				inven->item[i].amount++;
				return;
			}
		}
	}
	inven->item[inven->n_items] = item;
	inven->n_items++;
}

inline static void remove_item(struct inventory * inven,int i)
{
	for(;i<inven->n_items;i++)
		inven->item[i] = inven->item[i+1];
	inven->n_items--;
}

#endif
