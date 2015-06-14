#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "input.h"

struct player p;

struct map world_map;
struct model block;

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
/*automatically generated*/
struct model cube(){
	struct model model;
	struct vertice va[26];
	model.cardinality=12;
	model.poly = calloc(sizeof(struct polygon),model.cardinality);
	va[0] = (struct vertice) {{0.5000,-0.5000,-0.5000},{0.000000,-0.000000,-0.5000},{255,255,255}};
	va[1] = (struct vertice) {{-0.5000,-0.5000,-0.5000},{0.000000,-0.000000,-0.5000},{255,255,255}};
	va[2] = (struct vertice) {{-0.5000,0.5000,-0.5000},{0.000000,-0.000000,-0.5000},{255,255,255}};
	va[3] = (struct vertice) {{-0.5000,0.5000,0.5000},{0.000000,-0.000000,0.5000},{255,19,0}};
	va[4] = (struct vertice) {{-0.5000,-0.5000,0.5000},{0.000000,-0.000000,0.5000},{255,255,255}};
	va[5] = (struct vertice) {{0.5,-0.5001,0.5000},{0.000000,-0.000000,0.5000},{6,0,255}};
	va[6] = (struct vertice) {{0.5000,0.5,0.5000},{0.5000,-0.000001,0.000000},{58,255,0}};
	va[7] = (struct vertice) {{0.5,-0.5001,0.5000},{0.5000,-0.000001,0.000000},{6,0,255}};
	va[8] = (struct vertice) {{0.5000,-0.5000,-0.5000},{0.5000,-0.000001,0.000000},{255,255,255}};
	va[9] = (struct vertice) {{0.5,-0.5001,0.5000},{-0.000000,-0.5000,0.000000},{6,0,255}};
	va[10] = (struct vertice) {{-0.5000,-0.5000,0.5000},{-0.000000,-0.5000,0.000000},{255,255,255}};
	va[11] = (struct vertice) {{-0.5000,-0.5000,-0.5000},{-0.000000,-0.5000,0.000000},{255,255,255}};
	va[12] = (struct vertice) {{-0.5000,-0.5000,-0.5000},{-0.5000,0.000000,-0.000000},{255,255,255}};
	va[13] = (struct vertice) {{-0.5000,-0.5000,0.5000},{-0.5000,0.000000,-0.000000},{255,255,255}};
	va[14] = (struct vertice) {{-0.5000,0.5000,0.5000},{-0.5000,0.000000,-0.000000},{255,255,255}};
	va[15] = (struct vertice) {{0.5000,0.5000,-0.5000},{0.000000,0.5000,0.000000},{255,255,255}};
	va[16] = (struct vertice) {{-0.5000,0.5000,-0.5000},{0.000000,0.5000,0.000000},{255,255,255}};
	va[17] = (struct vertice) {{-0.5000,0.5000,0.5000},{0.000000,0.5000,0.000000},{255,255,255}};
	va[18] = (struct vertice) {{0.5000,0.5000,-0.5000},{0.000000,0.000000,-0.5000},{255,255,255}};
	va[19] = (struct vertice) {{0.5000,0.5,0.5000},{-0.000000,-0.000000,0.5000},{58,255,0}};
	va[20] = (struct vertice) {{0.5000,0.5000,-0.5000},{0.5000,0.000000,-0.000000},{255,255,255}};
	va[21] = (struct vertice) {{0.5000,0.5,0.5000},{0.5000,0.000000,-0.000000},{58,255,0}};
	va[22] = (struct vertice) {{0.5000,-0.5000,-0.5000},{0.5000,0.000000,-0.000000},{255,255,255}};
	va[23] = (struct vertice) {{0.5000,-0.5000,-0.5000},{-0.000000,-0.5000,-0.000000},{255,255,255}};
	va[24] = (struct vertice) {{-0.5000,0.5000,-0.5000},{-0.5000,0.000000,-0.000000},{255,255,255}};
	va[25] = (struct vertice) {{0.5000,0.5,0.5000},{0.000000,0.5000,0.000000},{255,255,255}};
	int i;
	for(i=0;i<26;i++) {
		va[i].c.x/=255;
		va[i].c.y/=255;
		va[i].c.z/=255;
	}
	model.poly[0] = (struct polygon) {{va[0],va[1],va[2]}};
	model.poly[1] = (struct polygon) {{va[3],va[4],va[5]}};
	model.poly[2] = (struct polygon) {{va[6],va[7],va[8]}};
	model.poly[3] = (struct polygon) {{va[9],va[10],va[11]}};
	model.poly[4] = (struct polygon) {{va[12],va[13],va[14]}};
	model.poly[5] = (struct polygon) {{va[15],va[16],va[17]}};
	model.poly[6] = (struct polygon) {{va[18],va[0],va[2]}};
	model.poly[7] = (struct polygon) {{va[19],va[3],va[5]}};
	model.poly[8] = (struct polygon) {{va[20],va[21],va[22]}};
	model.poly[9] = (struct polygon) {{va[23],va[9],va[11]}};
	model.poly[10] = (struct polygon) {{va[24],va[12],va[14]}};
	model.poly[11] = (struct polygon) {{va[25],va[15],va[17]}};
	return model;
}

/*automatically generated*/
struct model tetra(){
	struct model model;
	struct vertice va[12];
	model.cardinality=4;
	model.poly = calloc(sizeof(struct polygon),model.cardinality);
	va[0] = (struct vertice) {{0.985739,-1.043807,1.035874},{-0.333333,0.666667,0.666667},{0,21,255}};
	va[1] = (struct vertice) {{0.985739,0.956193,-0.964126},{-0.333333,0.666667,0.666667},{35,255,0}};
	va[2] = (struct vertice) {{-1.014262,-0.043807,-0.964126},{-0.333333,0.666667,0.666667},{255,0,16}};
	va[3] = (struct vertice) {{-1.014262,-0.043807,-0.964126},{-0.000000,0.000000,-1.000000},{255,0,16}};
	va[4] = (struct vertice) {{0.985739,0.956193,-0.964126},{-0.000000,0.000000,-1.000000},{35,255,0}};
	va[5] = (struct vertice) {{0.985739,-1.043807,-0.964126},{-0.000000,0.000000,-1.000000},{255,255,255}};
	va[6] = (struct vertice) {{0.985739,-1.043807,1.035874},{1.000000,0.000000,-0.000000},{0,21,255}};
	va[7] = (struct vertice) {{0.985739,-1.043807,-0.964126},{1.000000,0.000000,-0.000000},{255,255,255}};
	va[8] = (struct vertice) {{0.985739,0.956193,-0.964126},{1.000000,0.000000,-0.000000},{35,255,0}};
	va[9] = (struct vertice) {{0.985739,-1.043807,1.035874},{-0.447214,-0.894427,0.000000},{0,21,255}};
	va[10] = (struct vertice) {{-1.014262,-0.043807,-0.964126},{-0.447214,-0.894427,0.000000},{255,0,16}};
	va[11] = (struct vertice) {{0.985739,-1.043807,-0.964126},{-0.447214,-0.894427,0.000000},{255,255,255}};
	int i;
	for(i=0;i<12;i++) {
		va[i].c.x/=255;
		va[i].c.y/=255;
		va[i].c.z/=255;
	}
	model.poly[0] = (struct polygon) {{va[0],va[1],va[2]}};
	model.poly[1] = (struct polygon) {{va[3],va[4],va[5]}};
	model.poly[2] = (struct polygon) {{va[6],va[7],va[8]}};
	model.poly[3] = (struct polygon) {{va[9],va[10],va[11]}};
	return model;
}

int init_game(){
	int i;
	p.model=cube();
	p.location.x=0;p.location.y=0;p.location.z=0;
	world_map=mkmap();
	block=cube();
	for(i=0;i<NUMBER_OF_KEYS;i++)
		keys[i]=0;
	return 1;
}
