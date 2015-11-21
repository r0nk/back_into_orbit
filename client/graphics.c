#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "callbacks.h"
#include "game.h"
#include "inventory.h"
#include "poly.h"
#include "model.h"
#include "map.h"
#include "fonts.h"

GLFWwindow * window;
int window_width,window_height;

float ratio;

struct model p_model;
struct model d_model;
struct model ai_model;
struct model bullet_model;
struct model block_model;
struct model bp_model;
struct model rp_model;
struct model bflag_model;
struct model rflag_model;
struct model fh_model;
struct model shp_model;
struct model coin_model;

void init_models()
{
	p_model=pawn();
	d_model=player_model();
	ai_model=tetra();
	bullet_model=bullet();
	block_model=wall_block();
	bp_model=blue_pawn_model();
	rp_model=red_pawn_model();
	bflag_model=blue_flag_model();
	rflag_model=red_flag_model();
	fh_model = flag_holder_model();
	shp_model = shop_model();
	coin_model = gold_coin_model();
}

int init_window_lib()
{
	glfwSetErrorCallback(err_callback);
	if(!glfwInit())
		return -1;
	window = glfwCreateWindow(640, 480, "corvus", NULL, NULL);
	if(!window)
		return -2;
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, cursor_button_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &window_width, &window_height);
	return 0;
}

void init_gl()
{
	glViewport(0,0,window_width,window_height);
	ratio = window_width / (float) window_height;
	glClearColor(0.0,0.5,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}


int init_graphics()
{
	init_models();
	init_window_lib();
	init_gl();

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

void draw_model(struct model model,struct vector loc)
{
	unsigned int i;
	glPushMatrix();
	glTranslatef(loc.x,loc.y,loc.z);
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
	for(i=0;i<block_model.cardinality;i++)
		draw_poly(block_model.poly[i]);
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

void draw_unit(struct unit u)
{
	switch(u.type){
		case UNIT_TYPE_PLAYER:
			if(u.team==BLUE_TEAM)
				draw_model(bp_model,u.location);
			else if (u.team==RED_TEAM)
				draw_model(rp_model,u.location);
			else 
				draw_model(p_model,u.location);
			break;
		case UNIT_TYPE_SHOP:
			draw_model(shp_model,u.location);
			break;
		case UNIT_TYPE_COIN:
			draw_model(coin_model,u.location);
			break;
		case UNIT_TYPE_NEUTRAL_CREEP:
			draw_model(ai_model,u.location);
			break;
	}
}

void draw_letter(double x, double y, char bits[24])
{
	int i,j;
	struct polygon a,b;
	double w = FONT_BIT_WIDTH;
	double h = FONT_BIT_HEIGHT;
	double old_x = x;
	for(j=0;j<6;j++){
		for(i=3;i>=0;i--){
			a.v[0].p = (struct vector) {x    ,y    ,0.0};
			a.v[1].p = (struct vector) {x+ w ,y    ,0.0};
			a.v[2].p = (struct vector) {x    ,y+h  ,0.0};

			b.v[0].p = (struct vector) {x    ,y+h  ,0.0};
			b.v[1].p = (struct vector) {x+w  ,y+h  ,0.0};
			b.v[2].p = (struct vector) {x+w  ,y    ,0.0};

			b.v[0].c = (struct vector) {0,1,1};
			b.v[1].c = (struct vector) {0,1,1};
			b.v[2].c = (struct vector) {0,1,1};

			a.v[0].c = (struct vector) {0,1,1};
			a.v[1].c = (struct vector) {0,1,1};
			a.v[2].c = (struct vector) {0,1,1};

			a.v[0].n = normal(a.v[0].p);
			a.v[1].n = normal(a.v[1].p);
			a.v[2].n = normal(a.v[2].p);

			b.v[0].n = normal(b.v[0].p);
			b.v[1].n = normal(b.v[1].p);
			b.v[2].n = normal(b.v[2].p);

			if(bits[i + (j*4)]=='#'){
				draw_poly(a);
				draw_poly(b);
			}
			x+=w;
		}
		x=old_x;
		y-=h;
	}
}

void draw_models(struct game_state * gs)
{
	draw_map();
	int i;
	for(i=0;i<gs->n_players;i++){
		draw_unit(gs->game_player[i]);
	}
	for(i=0;i<gs->n_npcs;i++){
		draw_unit(gs->npc[i]);
	}
	for(i=0;i<gs->n_bullets;i++){
		draw_model(bullet_model,gs->bullet[i].location);
	}
	draw_model(rflag_model,gs->red.flag.location);
	draw_model(bflag_model,gs->blue.flag.location);
	draw_model(fh_model,gs->red.flag_starting);
	draw_model(fh_model,gs->blue.flag_starting);
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

void draw_inventory(struct unit u,double x, double y)
{
	int i;
	char str[30];
	for(i=0;i<MAX_INVENTORY_SPACE;i++){
		if(u.inventory.item[i].name !=NULL){
			sprintf(str,"[%i] %s",
					u.inventory.item[i].amount,
					u.inventory.item[i].name);
			draw_text(x,y-(i*FONT_HEIGHT),str);
		}
	}
}

void draw_score(struct game_state * gs,double x, double y)
{
	struct polygon a,b;
	a.v[0].p = (struct vector) {x+0.0,y+0.0,0.0};
	a.v[1].p = (struct vector) {x+1.0,y+0.0,0.0};
	a.v[2].p = (struct vector) {x+0.0,y+1.0,0.0};
	b.v[0].p = (struct vector) {x+2.0,y+0.0,0.0};
	b.v[1].p = (struct vector) {x+1.0,y+0.0,0.0};
	b.v[2].p = (struct vector) {x+2.0,y+1.0,0.0};

	if(gs->red.score>0)
		a.v[0].c = (struct vector) {1,0,0};
	else
		a.v[0].c = (struct vector) {0,0,0};
	if(gs->red.score>1)
		a.v[1].c = (struct vector) {1,0,0};
	else
		a.v[1].c = (struct vector) {0,0,0};
	if(gs->red.score>2)
		a.v[2].c = (struct vector) {1,0,0};
	else
		a.v[2].c = (struct vector) {0,0,0};

	if(gs->blue.score>0)
		b.v[0].c = (struct vector) {0,0,1};
	else
		b.v[0].c = (struct vector) {0,0,0};
	if(gs->blue.score>1)
		b.v[1].c = (struct vector) {0,0,1};
	else
		b.v[1].c = (struct vector) {0,0,0};
	if(gs->blue.score>2)
		b.v[2].c = (struct vector) {0,0,1};
	else
		b.v[2].c = (struct vector) {0,0,0};

	a.v[0].n = normal(a.v[0].p);
	a.v[1].n = normal(a.v[1].p);
	a.v[2].n = normal(a.v[2].p);
	b.v[0].n = normal(a.v[0].p);
	b.v[1].n = normal(a.v[1].p);
	b.v[2].n = normal(a.v[2].p);

	draw_poly(a);
	draw_poly(b);
}

void draw_hud(struct game_state * gs)
{
	glBegin(GL_TRIANGLES);

	draw_health_bar(gs->game_player[gs->current_player],0,-9);
	draw_score(gs,-1,9);
	draw_inventory(gs->game_player[gs->current_player],9,-5);

	//TODO draw this whenever we're close to the shop or whatever
	//draw_text(3,2,"Red Flag --> $ 20 ");

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
//	glFrustum(-1.0,1.0,-1.0f,1.0f,1.0f,100.0f);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	int zoom = 8;
	gluLookAt(gs->game_player[gs->current_player].location.x+zoom,
		gs->game_player[gs->current_player].location.y+(zoom*1.6),
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
