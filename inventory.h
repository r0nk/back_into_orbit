#ifndef INVENTORY
#define INVENTORY

#include <stdlib.h>
#include <stdio.h>

/* units are in the game world, items are in inventories */

struct item {
	char name[10];
	int active; /* whether or not the player activated this item */
	int stackable;
	int amount;
	void (*effect)(void * gs, double delta);
};

#define MAX_INVENTORY_SPACE 4

struct inventory {
	int n_items;
	struct item item[MAX_INVENTORY_SPACE];
};

inline static struct item regen_item()
{
	struct item item;
	sprintf(item.name,"regen");
	item.effect=NULL;
	item.stackable = 0;
	item.amount = 1;
	return item;
}

inline static struct item flag_item()
{
	struct item flag;
	sprintf(flag.name,"Flag");
	flag.effect=NULL;
	flag.stackable = 0;
	flag.amount = 1;
	return flag;
}

inline static struct item coin_item()
{
	struct item coin;
	sprintf(coin.name,"coin");
	coin.effect=NULL;
	coin.stackable = 1;
	coin.amount = 1;
	return coin;
}

 
#endif
