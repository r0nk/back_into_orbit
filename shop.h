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

static inline void dump_transaction(struct transaction t)
{
	printf("transaction item:\n");
	dump_item(t.item);
	printf("price: %i\n",t.price);
	printf("location:");
	dump_vector(t.location);
	printf("\n");
}


#define MAX_TRANSACTIONS 3

struct shop {
	struct vector keeper_location;
	struct transaction t[MAX_TRANSACTIONS];
};

static inline void dump_shop(struct shop shop)
{
	printf("keep_location:");
	dump_vector(shop.keeper_location);
	printf("\n");

	int i;
	for(i=0;i<MAX_TRANSACTIONS;i++){
		dump_transaction(shop.t[i]);
	}
}
 
#endif
