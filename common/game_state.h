#ifndef GAME_STATE
#define GAME_STATE

#include <game.h>
#include <stdio.h>

#define MAX_PLAYERS 10

struct game_state {
	int n_players;
	int current_player;
	struct vector player_location[MAX_PLAYERS];
};

static inline void dump_game_state(struct game_state gs)
{
	int i;
	printf("gs.n_players:%i\n",gs.n_players);
	printf("gs.current_player:%i\n",gs.current_player);
	for(i=0;i<gs.n_players;i++){
		printf("gs.player_location[%i]{",i);
		dump_vector(gs.player_location[i]);
		printf("}\n");
	}
}
 
#endif
