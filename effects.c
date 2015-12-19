#include <stdio.h>

#include "effects.h"
#include "map.h"
#include "audio.h"

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
	int a =((rand()%4)-2)*2;
	int b =((rand()%4)-2)*2;
	teledice_sound(a,b);
	gs->game_player.location.x+=a;
	gs->game_player.location.z+=b;
}

void item_effect(struct game_state * gs, struct item * item, double delta)
{
	if(item->cooldown>0){
		item->cooldown-=delta;
		return;
	}
	switch(item->type){
		case ITEM_REGEN:
			regen_effect(gs,delta);
			break;
		case ITEM_PUZZLE:
			if(item->active){
				puzzle_effect(gs,delta);
				item->cooldown=10;
			}
			break;
		case ITEM_TELEDICE:
			if(item->active){
				teledice_effect(gs,delta);
				item->cooldown=2;
			}
			break;
		case ITEM_TRIGGER:
			gs->game_player.flags|=HAS_TRIGGER;
			break;
		case ITEM_VAIL:
			gs->game_player.flags|=HAS_VAIL;
			break;
		case ITEM_VECTOR_FIELD:
			gs->game_player.flags|=HAS_VECTOR_FIELD;
			break;
		case ITEM_SHEILD:
			if(item->active){
				gs->game_player.speed=1.5;
				gs->game_player.resist=0.5;
			}else{
				gs->game_player.speed=3;
				gs->game_player.resist=1;
			}
			break;
		case ITEM_ENTROPY_BATTERY:
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
