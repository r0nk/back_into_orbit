#ifndef GAME_STATE
#define GAME_STATE

#include <game.h>

struct game_state {
	int n_players;
	struct vector player_location[10];
};
 
#endif
