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

int kite_effect(struct game_state * gs, double delta)
{
	int i;
	for(i=0;i<gs->n_npcs;i++){
		if(near(gs->game_player.location,gs->npc[i].location,5)){
			gs->game_player.speed=3;
			return 0;
		}
	}
	/*if we've reached this point, then the kite effect is active*/
	gs->game_player.speed=4;
	return 1;
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
		case ITEM_DASH:
			if(item->active){
				item->cooldown=5;
				gs->game_player.dash_timer=0.20;
			}
			break;
		case ITEM_KITE:
			if(kite_effect(gs,delta))
				item->cooldown=1;
			break;
		case ITEM_COIN:
			break;
		case ITEM_CAPACITOR:
			if(item->active){
				if(gs->game_player.damage<50){
					gs->game_player.damage+=10;
				}
				sin_sound(gs->game_player.damage);
				item->cooldown = 1;
			}
			break;
		case ITEM_ENTROPY_BATTERY:
		case ITEM_REMOTE:
		case ITEM_BEACON:
		case ITEM_BOX:
		case ITEM_ACCELERATOR:
		default:
			printf("ERR: unrecognized item effect: %i\n",item->type);
			break;
	}
}
