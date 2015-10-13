#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "callbacks.h"
#include "game.h"
#include "poly.h"
#include "map.h"

GLFWwindow * window;
int window_width,window_height;

float ratio;

struct model p_model;
struct model d_model;
struct model ai_model;

int init_graphics()
{
	glfwSetErrorCallback(err_callback);
	if(!glfwInit())
		return -1;
	window = glfwCreateWindow(640, 640, "corvus", NULL, NULL);
	if(!window)
		return -2;
	glfwMakeContextCurrent(window);

	p_model=pawn();
	d_model=player_model();
	ai_model=tetra();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, cursor_button_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &window_width, &window_height);
	glViewport(0,0,window_width,window_height);
	ratio = window_width / (float) window_height;
	glClearColor(0.5,0.2,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	return 0;
}

void deinit_graphics()
{
	glfwTerminate();
}

void draw_vertice(struct vertice v)
{
	glNormal3f(v.n.x, v.n.y, v.n.z);
	glColor3f( v.c.x, v.c.y, v.c.z);
	glVertex3f(v.p.x, v.p.y, v.p.z);
}

void draw_poly(struct polygon p)
{
	draw_vertice(p.v[0]);
	draw_vertice(p.v[1]);
	draw_vertice(p.v[2]);
}

void draw_model(struct model model,struct vector location)
{
	unsigned int i;
	glPushMatrix();
	glTranslatef(location.x,
			location.y,
			location.z);
	glBegin(GL_TRIANGLES);
	for(i=0;i<model.cardinality;i++)
		draw_poly(model.poly[i]);
	glEnd();
	glPopMatrix();
}

struct vector normal(struct vector v)
{
	struct vector n;
	n.z = v.x * v.y;
	n.x = v.y * v.z;
	n.y = v.z * v.x;
	return n;
}

void draw_block(int x, int y, int z)
{
	unsigned int i;
	glPushMatrix();
	glTranslatef((double)x,(double)y,(double)z);
	glBegin(GL_TRIANGLES);
	for(i=0;i<block.cardinality;i++)
		draw_poly(block.poly[i]);
	glEnd();
	glPopMatrix();
}

void draw_map()
{
	int x,z;
	for(x=0;x<100;x++){
		for(z=0;z<100;z++){
			if(world_map.tiles[x][z]){
				draw_block(x,0,z);
				draw_block(x,1,z);
			}
		}
	}
}

void draw_models(struct game_state * gs)
{
	draw_map();
	int i;
	for(i=0;i<gs->n_players;i++){
		draw_model(p_model,gs->game_player[i].location);
	}
	for(i=0;i<gs->n_npcs;i++){
		draw_model(ai_model,gs->npc[i].location);
	}
	draw_model(d_model,gs->game_player[gs->current_player].destination);
}

void draw_health_bar(struct unit u, int x, int y)
{
	struct polygon a,b;
	double h = u.health/50;
	a.v[0].p = (struct vector) {x+0.0,y+0.0,0.0};
	a.v[1].p = (struct vector) {x+ h ,y+0.0,0.0};
	a.v[2].p = (struct vector) {x+0.0,y+1.0,0.0};

	b.v[0].p = (struct vector) {x+0.0,y+1.0,0.0};
	b.v[1].p = (struct vector) {x+h  ,y+1.0,0.0};
	b.v[2].p = (struct vector) {x+h  ,y+0.0,0.0};

	b.v[0].c = (struct vector) {0,1,0};
	b.v[1].c = (struct vector) {0,1,0};
	b.v[2].c = (struct vector) {0,1,0};

	a.v[0].c = (struct vector) {0,1,0};
	a.v[1].c = (struct vector) {0,1,0};
	a.v[2].c = (struct vector) {0,1,0};

	a.v[0].n = normal(a.v[0].p);
	a.v[1].n = normal(a.v[1].p);
	a.v[2].n = normal(a.v[2].p);

	b.v[0].n = normal(b.v[0].p);
	b.v[1].n = normal(b.v[1].p);
	b.v[2].n = normal(b.v[2].p);

	draw_poly(a);
	draw_poly(b);
}

void draw_hud(struct game_state * gs)
{
	glBegin(GL_TRIANGLES);
	draw_health_bar(gs->game_player[gs->current_player],0,-9);
	glEnd();
}

void graphics_draw(struct game_state * gs)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	float frame = 10;
	glOrtho(-frame,frame,-frame,frame,0.0f,1000.0f);
	//glFrustum(-1.0,1.0,-1.0f,1.0f,1.0f,100.0f);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	int zoom = 8;
	gluLookAt(gs->game_player[gs->current_player].location.x+zoom,
			gs->game_player[gs->current_player].location.y+zoom,
			gs->game_player[gs->current_player].location.z+zoom,
			gs->game_player[gs->current_player].location.x,
			gs->game_player[gs->current_player].location.y,
			gs->game_player[gs->current_player].location.z,
			0,1,0);
	draw_models(gs);
	glPopMatrix();

	GLfloat HUD_Color[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, HUD_Color);

	glPushMatrix();
	gluLookAt(0,0,0, 0,0,1, 0,1,0);
	draw_hud(gs);
	glPopMatrix();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
