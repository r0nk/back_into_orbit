#ifndef INVENTORY
#define INVENTORY

/* units are in the game world, items are in inventories */

struct item {
	char  * name;
	void (*effects)(void);
};

#define MAX_INVENTORY_SPACE 6

struct inventory {
	int n_items;
	struct item item[MAX_INVENTORY_SPACE];
};

inline static struct item flag_item()
{
	struct item flag;
	flag.name="flag";
	flag.effects=NULL;
	return flag;
}
 
#endif
