#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "input.h"
#include "map.h"
#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "callbacks.h"
#include "game.h"
#include "inventory.h"
#include "poly.h"
#include "model.h"
#include "room.h"
#include "fonts.h"

GLFWwindow * window;
int window_width,window_height;

float ratio;

struct model p_model;
struct model d_model;
struct model ai_model;
struct model bullet_model;
struct model block_model;
struct model fh_model;
struct model shp_model;
struct model coin_model;
struct model door_model;
struct model floort_model;

void init_models()
{
	p_model=pawn();
	d_model=player_model();
	ai_model=scavenger();
	bullet_model=bullet();
	block_model=wall_block((struct vector) { 250,250,250});
	fh_model = flag_holder_model();
	shp_model = shop_model();
	coin_model = gold_coin_model();
	door_model = portal_model((struct vector) {0,255,255});
	floort_model=floor_tile();
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
	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //for wireframe mode
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

void draw_model(struct model model,struct vector loc,
		double angle, struct vector rotation)
{
	unsigned int i;
	glPushMatrix();
	glTranslatef(loc.x,loc.y,loc.z);
	glRotated(angle,rotation.x,rotation.y,rotation.z);
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

void draw_floor(int x, int y, int z){
	glPushMatrix();
	glTranslatef((double)x,(double)y,(double)z);
	glBegin(GL_TRIANGLES);
	draw_poly(floort_model.poly[0]);
	draw_poly(floort_model.poly[1]);
	glEnd();
	glPopMatrix();
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

void draw_doorway(struct doorway * d)
{
	door_model=portal_model(d->color);
	draw_model(door_model,(struct vector) {d->x,0,d->z},
				0.0, (struct vector){0,0,0});
}

void draw_room()
{
	int x,z,i;
	block_model=wall_block(world_map.current_room->color);
	for(x=0;x<100;x++){
		for(z=0;z<100;z++){
			if(world_map.current_room->layout.tiles[x][z]==LAYOUT_WALL){
				draw_block(x,0,z);
			}
			if(world_map.current_room->layout.tiles[x][z]){
				draw_floor(x,0,z);
			}
		}
	}
	for(i=0;i<(world_map.current_room->n_doorways);i++){
		draw_doorway(&world_map.current_room->doorway[i]);
	}
}

void draw_unit(struct unit u)
{
	glPushMatrix();
	switch(u.type){
		case UNIT_TYPE_PLAYER:
			draw_model(p_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_SHOP:
			draw_model(shp_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_COIN:
			draw_model(coin_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_NEUTRAL_CREEP:
			draw_model(ai_model,u.location,
					u.rotation_angle,u.rotation);
			break;
	}
	glPopMatrix();
}

void draw_letter(double x, double y, char bits[24], struct vector color)
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

			b.v[0].c = color;
			b.v[1].c = color;
			b.v[2].c = color;

			a.v[0].c = color;
			a.v[1].c = color;
			a.v[2].c = color;

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
	draw_room();
	int i;
	draw_unit(gs->game_player);
	for(i=0;i<gs->n_npcs;i++){
		draw_unit(gs->npc[i]);
	}
	for(i=0;i<gs->n_bullets;i++){
		draw_model(bullet_model,gs->bullet[i].location,
				0.0, (struct vector){0,0,0});
	}
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
			draw_text(x,y-(i*FONT_HEIGHT),str,
			    (struct vector) {u.inventory.item[i].active,1,1});
		}
	}
}

void draw_fps(double x, double y)
{
	char str[30];
	sprintf(str,"%i",(int)fps);
	draw_text(x,y,str,(struct vector) {0,1,1} );
}

void draw_hud(struct game_state * gs)
{
	GLfloat HUD_Color[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, HUD_Color);
	glPushMatrix();
	gluLookAt(0,0,0, 0,0,1, 0,1,0);
	glBegin(GL_TRIANGLES);

	draw_health_bar(gs->game_player,0,-7);
	draw_inventory(gs->game_player,9,-4.8);
	draw_fps(9,7);

	glEnd();
	glPopMatrix();
}

void graphics_draw(struct game_state * gs)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	float frame = 10;
	glOrtho(-frame,frame,-frame/1.333,frame/1.333,0.0f,1000.0f);
//	glFrustum(-1.0,1.0,-1.0f,1.0f,1.0f,100.0f);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	int zoom = 10;
	gluLookAt(gs->game_player.location.x+zoom,
		  gs->game_player.location.y+zoom,
		  gs->game_player.location.z+zoom,
		  gs->game_player.location.x,
		  gs->game_player.location.y,
		  gs->game_player.location.z,
		0,1,0);
	draw_models(gs);
	glPopMatrix();

	draw_hud(gs);

	glfwSwapBuffers(window);
	glfwPollEvents();
}
