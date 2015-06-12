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
	m.cardinality=8;
	m.poly = calloc(sizeof(struct polygon),m.cardinality);

	m.poly[0].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[0].point[1]=(struct point) {1.0,0.0,0.0};
	m.poly[0].point[2]=(struct point) {1.0,0.2,0.0};

	m.poly[1].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[1].point[1]=(struct point) {0.0,1.0,0.0};
	m.poly[1].point[2]=(struct point) {0.2,1.0,0.0};

	m.poly[2].point[0]=(struct point) {0.0,0.0,0.0};
	m.poly[2].point[1]=(struct point) {0.0,0.0,1.0};
	m.poly[2].point[2]=(struct point) {0.0,0.2,1.0};

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

struct model tetrahedron()
{
	struct model m;
	m.cardinality=4;
	m.poly = calloc(sizeof(struct polygon),m.cardinality);
	struct point a={1.0,1.0,1.0};
	struct point b={-1.0,-1.0,1.0};
	struct point c={-1.0,1.0,-1.0};
	struct point d={1.0,-1.0,-1.0};
	struct color ac={1.0,1.0,1.0};
	struct color bc={-1.0,-1.0,1.0};
	struct color cc={-1.0,1.0,-1.0};
	struct color dc={1.0,-1.0,-1.0};
	int i=0;
	m.poly[i].color[0]=(struct color) ac;
	m.poly[i].color[1]=(struct color) bc;
	m.poly[i].color[2]=(struct color) cc;
	m.poly[i].point[0]=a;
	m.poly[i].point[1]=b;
	m.poly[i].point[2]=c;
	i++;
	m.poly[i].color[0]=(struct color) bc;
	m.poly[i].color[1]=(struct color) ac;
	m.poly[i].color[2]=(struct color) dc;
	m.poly[i].point[0]=b;
	m.poly[i].point[1]=a;
	m.poly[i].point[2]=d;
	i++;
	m.poly[i].color[0]=(struct color) cc;
	m.poly[i].color[1]=(struct color) bc;
	m.poly[i].color[2]=(struct color) dc;
	m.poly[i].point[0]=c;
	m.poly[i].point[1]=b;
	m.poly[i].point[2]=d;
	i++;
	m.poly[i].color[0]=(struct color) dc;
	m.poly[i].color[1]=(struct color) ac;
	m.poly[i].color[2]=(struct color) cc;
	m.poly[i].point[0]=d;
	m.poly[i].point[1]=a;
	m.poly[i].point[2]=c;


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

	m.poly[0].color[0]=(struct color) {1.0,0.0,1.0};
	m.poly[0].color[1]=(struct color) {0.0,1.0,1.0};
	m.poly[0].color[2]=(struct color) {1.0,1.0,0.0};
	return m;
}

int init_game(){
	//TODO
	p.model=tetrahedron();
//	p.model=anchor();
	return 1;
}
