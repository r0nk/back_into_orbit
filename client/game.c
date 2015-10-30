#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "model.h"

struct map world_map;

struct game_state init_game()
{
	int i;
	struct game_state gs;
	gs.n_players=1;
	gs.current_player=0;
	for(i=0;i<MAX_PLAYERS;i++){
		gs.game_player[i].speed = 0;
		gs.game_player[i].location = (struct vector) {0,0,0};
		gs.game_player[i].destination = (struct vector) {0,0,0};
	}
	gs.game_player[gs.current_player].location.x=5;
	gs.game_player[gs.current_player].location.y=0;
	gs.game_player[gs.current_player].location.z=5;
	gs.game_player[gs.current_player].speed=3.0;
	gs.game_player[gs.current_player].health=100;
	world_map=mkmap("maps/condor.map");

	gs.n_bullets=0;
	return gs;
}
