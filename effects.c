#include <stdio.h>

#include "effects.h"
#include "map.h"

void regen_effect(struct game_state * gs, double delta){
	if((gs->game_player.health) < (gs->game_player.max_health)){
		gs->game_player.health+=delta;
	}
}

void puzzle_effect(struct game_state * gs, double delta)
{
	generate_edges(&world_map);
}

void teledice_effect(struct game_state * gs, double delta)
{
	gs->game_player.location.x+=((rand()%4)-2)*2;
	gs->game_player.location.z+=((rand()%4)-2)*2;
}

void item_effect(struct game_state * gs, struct item * item, double delta)
{
	if(item->cooldown>0){
		item->cooldown-=delta;
		return;
	}
	if(!item->active && !item->passive)
		return;
	switch(item->type){
		case ITEM_REGEN:
			regen_effect(gs,delta);
			break;
		case ITEM_PUZZLE:
			puzzle_effect(gs,delta);
			item->cooldown=10;
			break;
		case ITEM_TELEDICE:
			teledice_effect(gs,delta);
			item->cooldown=2;
			break;
		case ITEM_TRIGGER:
		case ITEM_VAIL:
		case ITEM_VECTOR_FEILD:
		case ITEM_ENTROPY_BATTERY:
		case ITEM_SHEILD:
		case ITEM_KITE:
		case ITEM_REMOTE:
		case ITEM_CAPACITOR:
		case ITEM_BEACON:
		case ITEM_BOX:
		case ITEM_COIN:
		case ITEM_ACCELERATOR:
		case ITEM_DASH:
		default:
			printf("ERR: unrecognized item effect: %i\n",item->type);
			break;
	}
}
