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
	npc.speed=0.0;
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
	npc.speed=3.1;
	npc.health=20;
	npc.damage=20;
	npc.location=location;
	npc.type = UNIT_TYPE_NEUTRAL_CREEP;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	return npc;
}

struct unit ranger_npc(struct vector location)
{
	struct unit npc;
	npc.speed=2.5;
	npc.health=10;
	npc.damage=10;
	npc.location=location;
	npc.type = UNIT_TYPE_RANGER;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	return npc;
}

struct unit boss_npc(struct vector location)
{
	struct unit npc;
	npc.speed=0;
	npc.health=100;
	npc.location=location;
	npc.type = UNIT_TYPE_BOSS;
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
	if(room->boss_room){
		add_npc(&room->gs,boss_npc(loc));
	}
}

struct unit init_player()
{
	struct unit player;
	player.location.x=5;
	player.location.y=0;
	player.location.z=5;
	player.speed=3.0;
	player.health=100;
	player.max_health=100;
	player.type=UNIT_TYPE_PLAYER;
	player.cooldown=1;
	player.damage=10;
	player.inventory.n_items=0;
	player.flags=0;
	player.resist=1;
	player.saying="";
	return player;
}

struct game_state init_game(struct room * room)
{
	room->gs.game_player=init_player();
	room->gs.n_bullets=0;
	room->gs.n_npcs=0;

	spawn_mobs(room);

	return room->gs;
}
