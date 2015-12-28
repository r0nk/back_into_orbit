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
#include "ui.h"

GLFWwindow * window;
int window_width,window_height;
float frame_x,frame_y;

float ratio;

struct button test_button;

struct model p_model;
struct model b_model;
struct model d_model;
struct model ai_model;
struct model ranger_model;
struct model bullet_model;
struct model block_model;
struct model fh_model;
struct model shp_model;
struct model coin_model;
struct model door_model;
struct model floort_model;
struct model stand_model;

struct model puzzle_item_model;
struct model regen_item_model;
struct model teledice_item_model;
struct model trigger_item_model;
struct model vail_item_model;
struct model dash_item_model;
struct model vector_field_item_model;
struct model shield_item_model;
struct model kite_item_model;
struct model capacitor_item_model;

void init_models()
{
	p_model=pawn((struct vector){1,1,1},(struct vector){0,1,1});
	d_model=player_model();
	ai_model=scavenger();
	ranger_model=pawn((struct vector){0.1,0.1,0.1},(struct vector){1,0,0});
	bullet_model=bullet();
	fh_model = flag_holder_model();
	shp_model = shopkeeper_model();
	coin_model = gold_coin_model();
	door_model = portal_model((struct vector) {0,255,255});
	floort_model=floor_tile((struct vector){1,0,1},(struct vector){0,0,0} );
	stand_model = pedestal_model();
	puzzle_item_model=puzzle_model();
	regen_item_model=regen_model();
	teledice_item_model=dice_model();
	trigger_item_model=trigger_model();
	vail_item_model=vail_model();
	dash_item_model=dash_model();
	vector_field_item_model=vector_field_model();
	shield_item_model=shield_model();
	kite_item_model=kite_model();
	capacitor_item_model=capacitor_model();
	b_model = boss_model();
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
//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //for wireframe mode
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

void draw_line(struct vector start, struct vector end, struct vector color)
{
	struct polygon a,b;
	float x,z;
	x=0.5;
	z=0.5;

	a.v[0].p = start;
	a.v[1].p = start;
	a.v[1].p.z+=z;
	a.v[1].p.x+=x;
	a.v[2].p = end;
	a.v[2].p.z+=z;
	a.v[2].p.x+=x;

	b.v[0].p = end;
	b.v[1].p = end;
	b.v[1].p.z+=z;
	b.v[1].p.x+=x;
	b.v[2].p = start;

	a.v[0].c = color;
	a.v[1].c = color;
	a.v[2].c = color;
	b.v[0].c = color;
	b.v[1].c = color;
	b.v[2].c = color;

	a.v[0].n = normal(a.v[0].p);
	a.v[1].n = normal(a.v[1].p);
	a.v[2].n = normal(a.v[2].p);
	b.v[0].n = normal(b.v[0].p);
	b.v[1].n = normal(b.v[1].p);
	b.v[2].n = normal(b.v[2].p);

	glBegin(GL_TRIANGLES);
	draw_poly(a);
	draw_poly(b);
	glEnd();
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
	if(!d->is_connected)
		return;
	door_model=portal_model(d->color);
	draw_model(door_model,d->location,
				0.0, (struct vector){0,0,0});
}

struct model * item_model(int item_id)
{
	switch(item_id){
		case ITEM_REGEN:
			return &regen_item_model;
		case ITEM_PUZZLE:
			return &puzzle_item_model;
		case ITEM_TELEDICE:
			return &teledice_item_model;
		case ITEM_TRIGGER:
			return &trigger_item_model;
		case ITEM_VAIL:
			return &vail_item_model;
		case ITEM_VECTOR_FIELD:
			return &vector_field_item_model;
		case ITEM_SHEILD:
			return &shield_item_model;
		case ITEM_KITE:
			return &kite_item_model;
		case ITEM_CAPACITOR:
			return &capacitor_item_model;
		case ITEM_DASH:
			return &dash_item_model;
		case ITEM_COIN:
			printf("incorrect call for coin\n");
		default:
			err(-74,"couldn't find item model for [%i] ",item_id);
			break;
	}
}

void draw_shop(struct shop * shop)
{
	draw_model(shp_model,shop->keeper_location,
				15.0, (struct vector){0,1,0});
	int i;
	for(i=0;i<MAX_TRANSACTIONS;i++){
		draw_model(stand_model,shop->t[i].location,
				0.0, (struct vector){0,1,0});
		if(!shop->t[i].sold){
			draw_model(*(item_model(shop->t[i].item.type)),
				shop->t[i].location,
				0.0, (struct vector){0,1,0});
		}
	}
}

void draw_room(struct room * room)
{
	int i;
	for(i=0;i<(room->n_doorways);i++){
		draw_doorway(&room->doorway[i]);
	}
	if(room->has_shop)
		draw_shop(&room->shop);	
	draw_model(room->model,(struct vector){0,0,0},
				0.0, (struct vector){0,0,0});
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
		case UNIT_TYPE_ITEM:
			draw_model(coin_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_NEUTRAL_CREEP:
			draw_model(ai_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_BOSS:
			draw_model(b_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_RANGER:
			draw_model(ranger_model,u.location,
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
	draw_room((world_map.current_room));
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
	struct vector c = {0,1,0};
	if(u.health<75)
		c = (struct vector) {1,1,0};
	if(u.health<25)
		c = (struct vector) {1,0,0};

	double h = u.health/40;
	a.v[0].p = (struct vector) {x-h  ,y+0.0,0.0};
	a.v[1].p = (struct vector) {x+0.0,y+0.0,0.0};
	a.v[2].p = (struct vector) {x-h  ,y+1.0,0.0};

	b.v[0].p = (struct vector) {x-h  ,y+1.0,0.0};
	b.v[1].p = (struct vector) {x+0.0,y+1.0,0.0};
	b.v[2].p = (struct vector) {x+0.0,y+0.0,0.0};

	b.v[0].c = c;
	b.v[1].c = c;
	b.v[2].c = c;
	a.v[0].c = c;
	a.v[1].c = c;
	a.v[2].c = c;

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
	for(i=0;i<u.inventory.n_items;i++){
		if(u.inventory.item[i].cooldown<=0){
			if(u.inventory.item[i].amount>1)
				sprintf(str,"%i| %s (%i)",i+1,get_item_name(u.inventory.item[i]),u.inventory.item[i].amount);
			else
				sprintf(str,"%i| %s ",i+1,get_item_name(u.inventory.item[i]));
			draw_text(x,y-(i*FONT_HEIGHT),str,
					(struct vector) {u.inventory.item[i].active,1,1});
		}else{
			sprintf(str,"%i| %s [%.2f]",i+1,
					get_item_name(u.inventory.item[i]),
					u.inventory.item[i].cooldown);
			draw_text(x,y-(i*FONT_HEIGHT),str,
					(struct vector) {0.5,0.5,0.5});
		}
	}
}

void draw_fps(double x, double y)
{
	char str[30];
	sprintf(str,"%i",(int)fps);
	draw_text(x,y,str,(struct vector) {0,1,1} );
}

void draw_button(struct button button)
{
	struct polygon a,b;

	struct vector l,s;
	l = button.location;
	s = button.size;

	a.v[0].p = (struct vector) {l.x+s.x,l.y+0.0,0.1};
	a.v[1].p = l;
	a.v[2].p = (struct vector) {l.x+s.x,l.y+s.y,0.1};

	b.v[0].p = (struct vector) {l.x+s.x,l.y+s.y,0.1};
	b.v[1].p = l;
	b.v[2].p = (struct vector) {l.x+0.0,l.y+s.y,0.1};

	b.v[0].c = button.color;
	b.v[1].c = button.color;
	b.v[2].c = button.color;
	a.v[0].c = button.color;
	a.v[1].c = button.color;
	a.v[2].c = button.color;

	a.v[0].n = normal(a.v[0].p);
	a.v[1].n = normal(a.v[1].p);
	a.v[2].n = normal(a.v[2].p);

	b.v[0].n = normal(b.v[0].p);
	b.v[1].n = normal(b.v[1].p);
	b.v[2].n = normal(b.v[2].p);

	draw_poly(a);
	draw_poly(b);
	draw_text(l.x+(s.x-0.7),l.y+0.7,button.text,(struct vector) {0,0,0} );
}

void draw_box(struct vector location)
{
	struct polygon a,b;

	struct vector color = (struct vector) {1,0,0};
	struct vector l,s;
	l = location;
	s = (struct vector) {0.1,0.1,0};

	a.v[0].p = (struct vector) {l.x+s.x,l.y+0.0,0.1};
	a.v[1].p = l;
	a.v[2].p = (struct vector) {l.x+s.x,l.y+s.y,0.1};

	b.v[0].p = (struct vector) {l.x+s.x,l.y+s.y,0.1};
	b.v[1].p = l;
	b.v[2].p = (struct vector) {l.x+0.0,l.y+s.y,0.1};

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

	draw_poly(a);
	draw_poly(b);
}

void draw_ui(struct ui * ui)
{
	int i;
	for(i=0;i<ui->n_buttons;i++){
		draw_button(ui->button[i]);
	}
}

void draw_prices(struct game_state * gs, struct shop * shop)
{
	int i;
	struct vector v;
	char price[10];
	for(i=0;i<MAX_TRANSACTIONS;i++){
		if(!shop->t[i].sold){
			sprintf(price,"%i",shop->t[i].price);
		}else{
			sprintf(price,"SOLD");
		}
		v=world_to_screen(gs,shop->t[i].location);
		draw_text(v.x,v.y,price,(struct vector) {1.0,0.7,0.0});
	}
}

void draw_sayings(struct game_state * gs)
{
	struct vector l;
	l= gs->game_player.location;
	l = world_to_screen(gs,l);
	if(gs->game_player.saying!=NULL)
		draw_text(l.x,l.y,gs->game_player.saying,(struct vector) {0,1,1});
}

void draw_hud(struct game_state * gs)
{
	GLfloat HUD_Color[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, HUD_Color);
	glPushMatrix();
	gluLookAt(0,0,0, 0,0,1, 0,1,0);
	glBegin(GL_TRIANGLES);

	draw_health_bar(gs->game_player,3,-7);
	draw_inventory(gs->game_player,9,-4.8);
	draw_fps(9,7);
	if(paused){
		draw_text(1,2,"paused",(struct vector) {0,1,1});
		draw_ui(ui);
	}
	if(world_map.current_room->has_shop)
		draw_prices(gs,&world_map.current_room->shop);

	draw_sayings(gs);

	glEnd();
	glPopMatrix();
}

void draw_game(struct game_state * gs)
{
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

}

void draw_main_menu()
{

	GLfloat HUD_Color[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, HUD_Color);
	glPushMatrix();
	gluLookAt(0,0,0, 0,0,1, 0,1,0);

	glBegin(GL_TRIANGLES);
	draw_ui(&main_menu_ui);
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

	frame_x=10;
	frame_y=10/1.333333;
	glOrtho(-frame_x,frame_x,-frame_y,frame_y,0.0f,1000.0f);
//	glFrustum(-1.0,1.0,-1.0f,1.0f,1.0f,100.0f);
	glMatrixMode(GL_MODELVIEW);

	if(!in_main_menu)
		draw_game(gs);
	else
		draw_main_menu();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
