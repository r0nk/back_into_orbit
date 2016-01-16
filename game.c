#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "game_state.h"
#include "room.h"
#include "model.h"
#include "map.h"

struct map world_map;

struct unit antenna_npc(struct vector location)
{
	struct unit npc={0}; 
	npc.speed=0.0;
	npc.cooldown = 2;
	npc.damage=0;
	npc.health=20;
	npc.location=location;
	npc.type = UNIT_TYPE_ANTENNA;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.hit_radius = 1;
	npc.poison_timer = 0.0;
	npc.score=300;
	return npc;
}

struct unit yo_npc(struct vector location)
{
	struct unit npc={0}; 
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
	struct unit npc={0};
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

struct unit sign_npc(struct vector location)
{
	struct unit npc={0};
	npc.speed=0.0;
	npc.health=200;
	npc.location=location;
	npc.type = UNIT_TYPE_SIGN;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	npc.hit_radius = 1.0;
	npc.poison_timer=0.0;
	npc.score=1;
	npc.saying="level 1";
	return npc;
}

struct unit scavenger_npc(struct vector location)
{
	struct unit npc={0};
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
	struct unit npc={0};
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
	struct unit npc={0};
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
	struct unit npc={0};
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
			if(room->gs.n_npcs==0){
				printf("coulnd't add mole_npc");
				exit(-44);
			}
			break;
		case 3:
			add_yoyo(&room->gs);
			if(room->gs.n_npcs==0){
				printf("coulnd't add yoyo");
				exit(-44);
			}
			break;
		default:
			printf("ERR:unrecognized boss type");
			break;
	}
}

void spawn_sign(struct game_state * gs,int n_room)
{
	struct unit s_npc = sign_npc((struct vector){14,0,3});

	struct unit s_m_npc = sign_npc((struct vector){14,0,8});
	struct unit s_c_npc = sign_npc((struct vector){10,0,8});
	s_m_npc.saying="WASD to move";
	s_c_npc.saying="click to shoot";

	switch(n_room){
		case 1:
			s_npc.saying="level 1";
			add_npc(gs,s_m_npc);
			add_npc(gs,s_c_npc);
			break;
		case 2:
			s_npc.saying="level 2";
			break;
		case 3:
			s_npc.saying="level 3";
			break;
	}
	add_npc(gs,s_npc);
}

void spawn_mob(struct room * room,int i, int j)
{
	struct vector loc;
	loc.x=i;loc.z=j;
	int r = rand()%3;
	switch(r){
		case 0:
			add_npc(&room->gs,ranger_npc(loc));
			break;
		case 1:
			add_npc(&room->gs,scavenger_npc(loc));
			break;
		case 2:
			add_npc(&room->gs,antenna_npc(loc));
			break;
	}
}

void spawn_mobs(struct room * room)
{
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(room->layout.tiles[i][j] == 's'){
				spawn_mob(room,i,j);
			}
		}
	}
	if(room->boss_room)
		spawn_boss(room);
	if(room->starting_room){
		spawn_sign(&room->gs,room->starting_room);
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

	room->gs.field = (struct field) {0};
	add_item(&(room->gs.game_player.inventory),teledice_item());
	return room->gs;
}
