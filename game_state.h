#ifndef GAME_STATE
#define GAME_STATE

#include <stdio.h>

#include "game.h"

#define MAX_NPCS 30
#define MAX_BULLETS 100

struct game_state {
	struct unit game_player;
	int n_npcs;
	struct unit npc[MAX_NPCS];
	int n_bullets;
	struct bullet bullet[MAX_BULLETS];
};

static inline void remove_bullet(struct game_state * gs, int i)
{
	for(;i<gs->n_bullets;i++)
		gs->bullet[i] = gs->bullet[i+1];
	gs->n_bullets--;
}

static inline void add_bullet(struct game_state * gs, struct bullet b)
{
	if(gs->n_bullets>MAX_BULLETS)
		return;
	gs->bullet[gs->n_bullets]=b;
	gs->n_bullets++;
}

static inline void remove_npc(struct game_state * gs, int i)
{
	for(;i<gs->n_npcs;i++)
		gs->npc[i] = gs->npc[i+1];
	gs->n_npcs--;
}

static inline void add_npc(struct game_state * gs, struct unit n)
{
	if(gs->n_npcs>MAX_NPCS)
		return;
	gs->npc[gs->n_npcs]=n;
	gs->n_npcs++;
}

struct game_state init_game();
 
#endif
