#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "input.h"
#include "model.h"

struct player main_player;

struct map world_map;
struct model block;

struct game_state init_game()
{
	int i;
	struct game_state gs;
	gs.n_players=1;
	gs.current_player=0;
	for(i=0;i<MAX_PLAYERS;i++);
		gs.player_location[i] = (struct vector) {0,0,0};
	main_player.location.x=5;
	main_player.location.y=0;
	main_player.location.z=5;
	gs.player_location[0] = main_player.location;
	main_player.speed=3.0;
	world_map=mkmap("maps/condor.map");
	block=cube();
	for(i=0;i<NUMBER_OF_KEYS;i++)
		keys[i]=0;
	return gs;
}
