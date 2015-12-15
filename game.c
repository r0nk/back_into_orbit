#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "game_state.h"
#include "room.h"
#include "model.h"
#include "map.h"

struct room world_room;
struct map world_map;

struct game_state init_game()
{
	struct game_state gs;
	struct unit npc;
	gs.game_player.location.x=5;
	gs.game_player.location.y=0;
	gs.game_player.location.z=5;
	gs.game_player.speed=3.0;
	gs.game_player.health=100;
	gs.game_player.type=UNIT_TYPE_PLAYER;
	gs.game_player.cooldown=1;
	gs.n_bullets=0;
	gs.n_npcs=0;

	npc.speed=2.0;
	npc.health=20;
	npc.location=(struct vector) {15,0,3};
	npc.destination=(struct vector) {15,0,3};
	npc.type = UNIT_TYPE_NEUTRAL_CREEP;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	add_npc(&gs,npc);

	return gs;
}
