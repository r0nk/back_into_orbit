#ifndef SHOP
#define SHOP

#include "inventory.h"
#include "poly.h"

struct transaction
{
	struct item item;
	int price;
	struct vector location;
};

#define MAX_TRANSACTIONS 3

struct shop {
	struct vector keeper_location;
	struct transaction t[MAX_TRANSACTIONS];
};
 
#endif
