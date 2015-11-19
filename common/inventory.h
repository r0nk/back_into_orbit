#ifndef INVENTORY
#define INVENTORY

/* units are in the game world, items are in inventories */

struct item {
	char name[10];
	int stackable;
	int amount;
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
	flag.stackable = 0;
	flag.amount = 1;
	return flag;
}

inline static struct item coin_item()
{
	struct item coin;
	coin.name[0]='c';
	coin.name[1]='o';
	coin.name[2]='i';
	coin.name[3]='n';
	coin.name[4]='\0';
	coin.effects=NULL;
	coin.stackable = 1;
	coin.amount = 1;
	return coin;
}
 
#endif
