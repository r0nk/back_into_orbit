#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "game_state.h"
#include "room.h"
#include "model.h"
#include "map.h"

struct map world_map;

struct unit item_npc(struct vector location,int item_id)
{
	struct unit npc;
	npc.speed=2.0;
	npc.health=2000;
	npc.location=location;
	npc.type = UNIT_TYPE_ITEM;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	return npc;
}

struct unit scavenger_npc(struct vector location)
{
	struct unit npc;
	npc.speed=2.0;
	npc.health=20;
	npc.location=location;
	npc.type = UNIT_TYPE_NEUTRAL_CREEP;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	return npc;
}

void spawn_mobs(struct room * room)
{
	struct vector loc = {0,0,0};
	int i,j;
	for(i=0;i<100;i++){
		for(j=0;j<100;j++){
			if(room->layout.tiles[i][j] == 's'){
				loc.x=i;loc.z=j;
				add_npc(&room->gs,scavenger_npc(loc));
			}
		}
	}
	loc.x=10;loc.z=10;
	add_npc(&room->gs,item_npc(loc,ITEM_REGEN));
}

struct game_state init_game(struct room * room)
{
	room->gs.game_player.location.x=5;
	room->gs.game_player.location.y=0;
	room->gs.game_player.location.z=5;
	room->gs.game_player.speed=3.0;
	room->gs.game_player.health=100;
	room->gs.game_player.max_health=100;
	room->gs.game_player.type=UNIT_TYPE_PLAYER;
	room->gs.game_player.cooldown=1;
	room->gs.game_player.inventory.n_items=0;
	room->gs.n_bullets=0;
	room->gs.n_npcs=0;

	add_item(&(room->gs.game_player.inventory),puzzle_item());
	spawn_mobs(room);

	return room->gs;
}
