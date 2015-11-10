#ifndef INVENTORY
#define INVENTORY

/* units are in the game world, items are in inventories */

struct item {
	char name[16];
	void (*effects)(void);
}

#define MAX_INVENTORY_SPACE 6

struct inventory {
	int n_items;
	struct item i[MAX_INVENTORY_SPACE];
}
 
#endif
