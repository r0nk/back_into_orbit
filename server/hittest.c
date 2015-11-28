#include <game_state.h>
#include "hittest.h"

int near(struct vector a, struct vector b,double r)
{
	if(( a.x > (b.x - r)  && a.x < b.x + r) &&
			(a.z > (b.z-r) && a.z < (b.z+r))){
		return 1;
	}
	return 0;
}

int player_hittest(struct game_state * gs, struct vector b)
{
	int i;
	for(i=0;i<gs->n_players;i++){
		if(near(gs->game_player[i].location,b,1))
			return i;
	}
	return -1;
}

int npc_hittest(struct game_state * gs, struct vector b)
{
	int i;
	for(i=0;i<gs->n_npcs;i++){
		if(near(gs->npc[i].location,b,1))
			return i;
	}
	return -1;
}
