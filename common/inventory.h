#ifndef INVENTORY
#define INVENTORY

/* units are in the game world, items are in inventories */

struct item {
	char name[10];
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
	flag.name[0]='F';
	flag.name[1]='l';
	flag.name[2]='a';
	flag.name[3]='g';
	flag.name[4]='\0';
	flag.effects=NULL;
	return flag;
}
 
#endif
