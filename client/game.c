#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "input.h"
#include "model.h"

struct player main_player;

struct map world_map;
struct model block;

int init_game(){
	int i;
	main_player.model=player_model();
	main_player.dest_model=tetra();
	main_player.location.x=5;
	main_player.location.y=0;
	main_player.location.z=5;
	main_player.speed=3.0;
	world_map=mkmap();
	block=cube();
	for(i=0;i<NUMBER_OF_KEYS;i++)
		keys[i]=0;
	return 1;
}
