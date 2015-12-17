#include <stdio.h>

#include "effects.h"
#include "map.h"

void item_effect(struct game_state * gs, struct item item, double delta)
{
	if(item.active)
		item.effect(gs,delta);
}

void regen_effect(struct game_state * gs, double delta){
	printf("regeneratoing %f\n",delta);
	world_map.current_room->gs.game_player.health+=delta;
}
