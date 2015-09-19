#ifndef GAME_STATE
#define GAME_STATE

#include <game.h>

struct game_state {
	int n_players;
	struct vector player_location[10];
};

static inline void dump_game_state(struct game_state gs)
{
	int i;
	printf("gs.n_players:%i\n",gs.n_players);
	for(i=0;i<gs.n_players;i++){
		printf("gs.player_location[%i]:",i);
		dump_vector(gs.player_location[i]);
		printf("\n");
	}
}
 
#endif
