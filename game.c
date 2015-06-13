#include <stdlib.h>
#include "game.h"

struct player p;

int game_tick(){
	/* update each of the players based on their state */
	return 1;
}

struct model player_model()
{
	struct model m;
	m.cardinality=1;
	m.poly = calloc(sizeof(struct polygon),m.cardinality);
	m.poly[0].v[0].p=(struct vector){0.0,0.0,0.0};
	m.poly[0].v[1].p=(struct vector){0.5,0.5,0.0};
	m.poly[0].v[2].p=(struct vector){0.0,0.5,0.5};

	m.poly[0].v[0].c=(struct vector){1.0,0.0,1.0};
	m.poly[0].v[1].c=(struct vector){0.0,1.0,1.0};
	m.poly[0].v[2].c=(struct vector){1.0,1.0,0.0};
	return m;
}

int init_game(){
	//TODO
	p.model=player_model();
	return 1;
}
