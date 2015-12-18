#include <stdio.h>

#include "effects.h"
#include "map.h"

void item_effect(struct game_state * gs, struct item item, double delta)
{

	if(!item.active && !item.passive)
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
	if((gs->game_player.health) < (gs->game_player.max_health)){
		gs->game_player.health+=delta;
	}
}
