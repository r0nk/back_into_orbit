#include <stdlib.h>
#include "game.h"

struct player p;

int game_tick(){
	/* update each of the players based on their state */
	return 1;
}

struct model anchor()
{
	struct model m;
	m.cardinality=4;
	int i;
	m.poly = calloc(sizeof(struct polygon),m.cardinality);

	m.poly[0].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[0].point[1]=(struct point) {0.5,0.0,0.0};
	m.poly[0].point[2]=(struct point) {0.5,0.2,0.0};

	m.poly[1].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[1].point[1]=(struct point) {0.0,0.5,0.0};
	m.poly[1].point[2]=(struct point) {0.2,0.5,0.0};

	m.poly[2].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[2].point[1]=(struct point) {0.0,0.0,0.5};
	m.poly[2].point[2]=(struct point) {0.0,0.2,0.5};

	m.poly[0].color[0]=(struct color) {1.0,0.0,0.0};
	m.poly[0].color[1]=(struct color) {1.0,0.0,0.0};
	m.poly[0].color[2]=(struct color) {1.0,0.0,0.0};

	m.poly[1].color[0]=(struct color) {0.0,1.0,0.0};
	m.poly[1].color[1]=(struct color) {0.0,1.0,0.0};
	m.poly[1].color[2]=(struct color) {0.0,1.0,0.0};

	m.poly[2].color[0]=(struct color) {0.0,0.0,1.0};
	m.poly[2].color[1]=(struct color) {0.0,0.0,1.0};
	m.poly[2].color[2]=(struct color) {0.0,0.0,1.0};


	return m;
}

struct model player_model()
{
	struct model m;
	m.cardinality=1;
	m.poly = calloc(sizeof(struct polygon),m.cardinality);
	m.poly[0].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[0].point[1]=(struct point) {0.5,0.5,0.0};
	m.poly[0].point[2]=(struct point) {0.0,0.5,0.5};

	m.poly[0].color[0]=(struct color) {1.0,1.0,1.0};
	m.poly[0].color[1]=(struct color) {1.0,1.0,1.0};
	m.poly[0].color[2]=(struct color) {1.0,1.0,0.5};
	return m;
}

int init_game(){
	//TODO
//	p.model=player_model();
	p.model=anchor();
	return 1;
}
