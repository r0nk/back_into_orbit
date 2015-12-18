#include <stdio.h>

#include "effects.h"
#include "map.h"

void item_effect(struct game_state * gs, struct item item, double delta)
{

	printf("item effect called, active:%i\n",item.active);
	if(!item.active)
		return;
	switch(item.type){
		case ITEM_REGEN:
			regen_effect(gs,delta);
			break;
		default:
			printf("ERR: unrecognized item effect: %i\n",item.type);
			break;
	}
}

void regen_effect(struct game_state * gs, double delta){
	printf("regeneratoing %f\n",delta);
	gs->game_player.health+=delta;
}
