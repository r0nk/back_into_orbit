#ifndef GAME_LOGIC
#define GAME_LOGIC
#include <poly.h>
#include <game_state.h>

void npc_update(struct game_state * gs,double delta);
void fire_bullet(struct game_state * gs,struct vector starting, struct vector direction);
void bullet_update(struct game_state * gs, double delta);
void player_update(struct game_state * gs,double delta);
void flag_update(struct game_state * gs,double delta);

#endif
