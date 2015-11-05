#ifndef GAME_STATE
#define GAME_STATE

#include <game.h>
#include <stdio.h>

#define MAX_PLAYERS 10
#define MAX_NPCS 30
#define MAX_BULLETS 100

struct game_state {
	struct unit red_flag;
	struct unit blue_flag;
	int n_players;
	int current_player;
	struct unit game_player[MAX_PLAYERS];
	int n_npcs;
	struct unit npc[MAX_NPCS];
	int n_bullets;
	struct bullet bullet[MAX_BULLETS];
};

static inline void add_bullet(struct game_state * gs, struct bullet b)
{
	if(gs->n_bullets>MAX_BULLETS)
		return;
	gs->bullet[gs->n_bullets]=b;
	gs->n_bullets++;
}

static inline void dump_game_state(struct game_state gs)
{
	int i;
	printf("----- dumping game state: -----");
	printf("gs.n_players:%i\n",gs.n_players);
	printf("gs.current_player:%i\n",gs.current_player);
	for(i=0;i<gs.n_players;i++){
		printf("gs.game_player[%i]{\n",i);
		dump_unit(gs.game_player[i]);
		printf("\n}\n");
	}

	printf("gs.n_npc:%i\n",gs.n_npcs);
	for(i=0;i<gs.n_npcs;i++){
		printf("gs.npc[%i]{\n",i);
		dump_unit(gs.npc[i]);
		printf("\n}\n");
	}

	printf("gs.n_bullets:%i\n",gs.n_bullets);
	for(i=0;i<gs.n_bullets;i++){
		printf("gs.bullets[%i]{\n",i);
		dump_bullet(gs.bullet[i]);
		printf("\n}\n");
	}
}

struct game_state init_game();
 
#endif
