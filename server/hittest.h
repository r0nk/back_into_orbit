#ifndef HITTEST
#define HITTEST 1
 
int near(struct vector a, struct vector b,double r);
int player_hittest(struct game_state * gs, struct vector b);
int npc_hittest(struct game_state * gs, struct vector b);

#endif
