#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "game_state.h"
#include "room.h"
#include "model.h"
#include "map.h"

struct map world_map;

struct unit yo_npc(struct vector location)
{
	struct unit npc;
	npc.speed=5.0;
	npc.damage=40;
	npc.health=100;
	npc.location=location;
	npc.type = UNIT_TYPE_YO;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.hit_radius = 1;
	npc.poison_timer = 0.0;
	npc.score=300;
	return npc;
}

struct unit item_npc(struct vector location, int item_id)
{
	struct unit npc;
	npc.speed=0.0;
	npc.health=2000;
	npc.location=location;
	npc.type = UNIT_TYPE_ITEM;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.hit_radius = 0;
	npc.poison_timer=0.0;
	npc.score=1;
	return npc;
}

struct unit scavenger_npc(struct vector location)
{
	struct unit npc;
	npc.speed=3.5;
	npc.health=20;
	npc.damage=20;
	npc.location=location;
	npc.type = UNIT_TYPE_NEUTRAL_CREEP;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.poison_timer = 0.0;
	npc.hit_radius = 1.0;
	npc.score=5;
	return npc;
}

struct unit ranger_npc(struct vector location)
{
	struct unit npc;
	npc.speed=2.5;
	npc.cooldown=1;
	npc.health=10;
	npc.damage=10;
	npc.location=location;
	npc.type = UNIT_TYPE_RANGER;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.poison_timer = 0.0;
	npc.hit_radius = 1.0;
	npc.score=10;
	return npc;
}

struct unit mole_npc(struct vector location)
{
	struct unit npc;
	npc.speed=0;
	npc.damage=10;
	npc.health=150;
	npc.location=location;
	npc.type = UNIT_TYPE_MOLE;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.hit_radius = 1.0;
	npc.poison_timer = 0.0;
	npc.score=100;
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
	npc.hit_radius = 2.0;
	npc.poison_timer = 0.0;
	npc.score=100;
	return npc;
}

/* adds both the yoyo units */
void add_yoyo(struct game_state * gs)
{
	struct unit y,z;
	y = yo_npc((struct vector) {1,0,1});
	z = yo_npc((struct vector) {11,0,1});

	/*this (hacky thing) connects the yo's together*/
	y.connected_to=gs->n_npcs+1;
	z.connected_to=gs->n_npcs;

	add_npc(gs,y);
	add_npc(gs,z);
}

void spawn_boss(struct room * room)
{
	struct vector loc = {10,0,10};
	switch(room->boss_room)
	{
		case 1:
			add_npc(&room->gs,boss_npc(loc));
			break;
		case 2:
			add_npc(&room->gs,mole_npc(loc));
			if(room->gs.n_npcs==0)
				err(-44,"couldn't add mole_npc");
			break;
		case 3:
			add_yoyo(&room->gs);
			if(room->gs.n_npcs==0)
				err(-43,"couldn't add yoyo_npc");
			break;
		default:
			printf("ERR:unrecognized boss type");
			break;
	}
}

void spawn_mobs(struct room * room)
{
	struct vector loc = {0,0,0};
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(room->layout.tiles[i][j] == 's'){
				loc.x=i;loc.z=j;
				if(rand()%2)
					add_npc(&room->gs,ranger_npc(loc));
				else
					add_npc(&room->gs,scavenger_npc(loc));
			}
		}
	}
	if(room->boss_room)
		spawn_boss(room);
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
	player.hit_radius = 2.0;
	player.score=0;
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
