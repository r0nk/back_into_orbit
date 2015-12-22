#ifndef INVENTORY
#define INVENTORY

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

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

inline static void dump_item(struct item i)
{
	printf("type:%i\n",i.type);
	printf("cooldown:%f\n",i.cooldown);
	printf("duration:%f\n",i.duration);
	printf("active:%i\n",i.active);
	printf("stackable:%i\n",i.stackable);
	printf("passive:%i\n",i.passive);
	printf("amount:%i\n",i.amount);
}

#define ITEM_REGEN 1
#define ITEM_PUZZLE 2
#define ITEM_TELEDICE 3
#define ITEM_TRIGGER 4
#define ITEM_VAIL 5
#define ITEM_VECTOR_FIELD 6
#define ITEM_SHEILD 7
#define ITEM_KITE 8
#define ITEM_CAPACITOR 9
#define ITEM_COIN 10
#define ITEM_DASH 11

#define N_ITEMS 11

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
		case ITEM_SHEILD:
			return "sheild";
		case ITEM_KITE:
			return "kite";
		case ITEM_CAPACITOR:
			return "capacitor";
		case ITEM_COIN:
			return "coin";
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

inline static struct item pick_item()
{
	int r = 1+(rand()%N_ITEMS);
	switch(r){
		case ITEM_REGEN:
			return regen_item();
		case ITEM_PUZZLE:
			return puzzle_item();
		case ITEM_TELEDICE:
			return teledice_item();
		case ITEM_TRIGGER:
			return trigger_item();
		case ITEM_VAIL:
			return vail_item();
		case ITEM_VECTOR_FIELD:
			return vector_field_item();;
		case ITEM_SHEILD:
			return sheild_item();
		case ITEM_KITE:
			return kite_item();
		case ITEM_CAPACITOR:
			return capacitor_item();
		case ITEM_COIN:
		case ITEM_DASH:
			return dash_item();
		default:
			err(-72,"failed to pick item, r=%i ",r);
			break;
	}
}

static inline int number_of_coins(struct inventory inven)
{
	int i;
	for(i=0;i<inven.n_items;i++){
		if(inven.item[i].type==ITEM_COIN)
			return inven.item[i].amount;
	}
	return 0;
}

#endif
