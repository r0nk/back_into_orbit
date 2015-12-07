#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "game_state.h"
#include "room.h"
#include "model.h"

struct room world_room;

struct game_state init_game()
{
	struct game_state gs;
	gs.game_player.location.x=5;
	gs.game_player.location.y=0;
	gs.game_player.location.z=5;
	gs.game_player.speed=2.5;
	gs.game_player.health=100;
	gs.game_player.type=UNIT_TYPE_PLAYER;
	world_room=mkroom("rooms/simple.room");

	gs.n_bullets=0;
	return gs;
}
