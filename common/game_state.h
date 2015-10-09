#ifndef GAME_STATE
#define GAME_STATE

#include <game.h>
#include <stdio.h>

#define MAX_PLAYERS 10
#define MAX_NPCS 30

struct game_state {
	int n_players;
	int current_player;
	struct unit game_player[MAX_PLAYERS];
	int n_npcs;
	struct unit npc[MAX_NPCS];
};

static inline void dump_game_state(struct game_state gs)
{
	int i;
	printf("gs.n_players:%i\n",gs.n_players);
	printf("gs.current_player:%i\n",gs.current_player);
	for(i=0;i<gs.n_players;i++){
		printf("gs.game_player[%i]{\n",i);
		dump_unit(gs.game_player[i]);
		printf("\n}\n");
	}
}

struct game_state init_game();
 
#endif
