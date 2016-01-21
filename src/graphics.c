#include <GL/glu.h>
#include <SDL.h>

#include "model_reader.h"
#include "input.h"
#include "field.h"
#include "pathfinding.h"
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

SDL_Window * window;
SDL_GLContext glcontext;
int window_width,window_height;
float frame_x,frame_y;

float ratio;

struct button test_button;

struct model p_model;
struct model b_model;
struct model ant_model;
struct model mole_boss_model;
struct model yo_boss_model;
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

struct model mm_planet_model;

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
	mm_planet_model=main_menu_planet_model();
	mole_boss_model = mole_model();
	yo_boss_model = yo_model((struct vector) {16,16,16},(struct vector) {250,0,0});
	ant_model = antenna_model();
}

int init_window_lib()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
		fprintf(stderr,"\nUnable to initialize SDL:  %s\n",
				SDL_GetError()
		       );
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("Back Into Orbit",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_OPENGL );
	if(!window)
		printf("!window\n");
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(!glcontext)
		printf("!glcontext\n");

	SDL_GL_GetDrawableSize(window,&window_width,&window_height);

	return 0;
}

/* old glfw version
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
*/

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
	SDL_Quit();
}

static inline void draw_vertice(struct vertice v)
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

void draw_2line(struct vector start, struct vector end, 
		struct vector c1,struct vector c2)
{
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(c1.x, c1.y, c1.z);
	glVertex3d(start.x,start.y,start.z);
	glColor3f(c2.x, c2.y, c2.z);
	glVertex3d(end.x,end.y,end.z);
	glEnd();
}

void draw_line(struct vector start, struct vector end, struct vector color)
{
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	glVertex3d(start.x,start.y,start.z);
	glVertex3d(end.x,end.y,end.z);
	glEnd();
}

void draw_stippled_line(struct vector start, struct vector end, 
		struct vector color)
{
	glPushAttrib(GL_ENABLE_BIT);

	glLineStipple(1,0xF000);
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(3.0);
	glColor3f(color.x, color.y, color.z);
	glBegin(GL_LINES);
	glVertex3d(start.x,start.y,start.z);
	glVertex3d(end.x,end.y,end.z);
	glEnd();
	
	glPopAttrib();
}

void draw_field(struct field * field)
{
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(!field->cell[i][j].y)
				continue;
			struct vector s,q,e,z,c,w,a,d,x;
			s = (struct vector) {i,field->cell[i][j].y,j};
			w = (struct vector) {i,field->cell[i][j+1].y,j+1};
			a = (struct vector) {i-1,field->cell[i-1][j].y,j};
			d = (struct vector) {i+1,field->cell[i+1][j].y,j};
			x = (struct vector) {i,field->cell[i][j-1].y,j-1};

			/*floor color, (for dat blend)*/
			double f = 0.6745; 
			struct vector cs = (struct vector) {f,s.y+f,s.y+f};
			draw_2line(s,w,cs,(struct vector) {f,w.y+f,w.y+f});
			draw_2line(s,a,cs,(struct vector) {f,a.y+f,a.y+f});
			draw_2line(s,d,cs,(struct vector) {f,d.y+f,d.y+f});
			draw_2line(s,x,cs,(struct vector) {f,x.y+f,x.y+f});
		}
	}
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
			printf("couldn't find item model for [%i]\n",item_id);
			exit(-74);
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
	draw_field(&room->gs.field);
}

void draw_yoyo(struct unit u){
	struct vector a,b;
	struct game_state * gs = &(world_map.current_room->gs);
	a = u.location;
	b = gs->npc[u.connected_to].location;
	a.y+=1;
	b.y+=1;
	draw_line(a,b,(struct vector) {1,0,0});
	draw_model(yo_boss_model,u.location,u.rotation_angle+90.0,u.rotation);
}

void draw_unit_saying(struct game_state * gs, struct unit * u)
{
	struct vector l;
	l = world_to_screen(gs,u->location);
	if(u->saying!=NULL){
		if(u->type==UNIT_TYPE_SIGN)
			draw_text(l.x+1,l.y+1,u->saying,(struct vector) {0,1,1});
		else
			draw_text(l.x,l.y,u->saying,(struct vector) {0,1,1});
	}
}

void draw_unit(struct game_state * gs, struct unit u)
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
		case UNIT_TYPE_MOLE:
			draw_model(mole_boss_model,u.location,
					u.rotation_angle+90.0,u.rotation);
			break;
		case UNIT_TYPE_YO:
			draw_yoyo(u);
			break;
		case UNIT_TYPE_SIGN:
			draw_model(fh_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		case UNIT_TYPE_ANTENNA:
			draw_model(ant_model,u.location,
					u.rotation_angle,u.rotation);
			break;
		default:
			printf("unrecognized model type, not drawing\n");
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

void draw_path(struct path path)
{
	int i;
	for(i=0;i<path.n_interpoints;i++){
		if(path.interpoint[i].x == 0 && path.interpoint[i].z==0)
			break;
		if(path.interpoint[i+1].x == 0 && path.interpoint[i+1].z==0)
			break;
		path.interpoint[i].y=0.1;
		path.interpoint[i+1].y=0.1;
		draw_line(path.interpoint[i],path.interpoint[i+1],(struct vector) {1,1,1});
	}
}

void draw_bullet(struct bullet b)
{
	struct vector v = b.location;
	struct vector w = b.location;
	struct vector color = {1,1,0};
	v.x += b.direction.x;
	v.y += b.direction.y+0.2;
	v.z += b.direction.z;
	w.y+=0.2;
	glLineWidth(8.0);
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	glVertex3d(w.x,w.y,w.z);
	glVertex3d(v.x,v.y,v.z);
	glEnd();
}

void draw_models(struct game_state * gs)
{
	draw_room((world_map.current_room));
	int i;
	draw_unit(gs,gs->game_player);
	for(i=0;i<gs->n_npcs;i++){
		draw_unit(gs,gs->npc[i]);
	}
	for(i=0;i<gs->n_bullets;i++){
		draw_bullet(gs->bullet[i]);
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
	if(u.health<0)
		return;

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

void draw_text_field(struct text_field t)
{
	struct vector l = t.location;
	draw_text(l.x,l.y,t.text,t.color);
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
	for(i=0;i<ui->n_text_fields;i++){
		draw_text_field(ui->text_field[i]);
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
	draw_unit_saying(gs,&gs->game_player);
	int i;
	for(i=0;i<(gs->n_npcs);i++){
		draw_unit_saying(gs,&gs->npc[i]);
	}
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

#define MAX_STARS 100

struct vector menu_star[MAX_STARS];
int stars_generated=0;

void generate_stars()
{
	int i;
	for(i=0;i<MAX_STARS;i++){
		menu_star[i] = (struct vector)
			{((rand()%1000)/50.0)-10,((rand()%1000)/50.0)-10,10};
	}
	stars_generated=1;
}

void draw_stars()
{
	float x=0.05;
	float y=0.05;
	if(!stars_generated)
		generate_stars();

	struct vector color = (struct vector) {1,1,1};
	int i;
	glBegin(GL_TRIANGLES);
	for(i=0;i<MAX_STARS;i++){
		struct polygon a;

		a.v[0].p = menu_star[i];
		a.v[1].p = menu_star[i];
		a.v[1].p.x+=x;
		a.v[2].p = menu_star[i];
		a.v[2].p.y+=y;
		a.v[2].p.x+=x;

		a.v[0].c = color;
		a.v[1].c = color;
		a.v[2].c = color;

		a.v[0].n = normal(a.v[0].p);
		a.v[1].n = normal(a.v[1].p);
		a.v[2].n = normal(a.v[2].p);

		draw_poly(a);

	}
	glEnd();
}

struct vector get_selection_v(struct vector starting){
	struct vector v = starting;
	v.z++;
	struct vector pmv = pixel_to_screen(pi.mouse_x,pi.mouse_y);

	if(pmv.y<2.5 && pmv.y > -1.5)
		v.y=pmv.y;
	else
		v.y=2;
	return v;
}

void draw_selection_plane(struct vector starting, struct vector path_color)
{
	struct vector c = {0,0,0};
	struct vector v = get_selection_v(starting);
	double ship_scale = 0.3;
	draw_line(starting,v,path_color);

	struct polygon a,b;

	a.v[0].p = v;
	a.v[1].p = v;
	a.v[2].p = v;

	a.v[0].p.y+=1*ship_scale;
	a.v[1].p.x-=1.4141*ship_scale;
	a.v[2].p.y-=1*ship_scale;

	a.v[0].p.x+=0.2;
	a.v[1].p.x+=0.2;
	a.v[2].p.x+=0.2;

	a.v[0].p.z=0;
	a.v[1].p.z=0;
	a.v[2].p.z=0;

	a.v[0].c = c;
	a.v[1].c = c;
	a.v[2].c = c;

	a.v[0].n = normal(a.v[0].p);
	a.v[1].n = normal(a.v[1].p);
	a.v[2].n = normal(a.v[2].p);

	b.v[0].p = v;
	b.v[1].p = v;
	b.v[2].p = v;

	b.v[0].p.z=1;
	b.v[1].p.z=1;
	b.v[2].p.z=1;

	b.v[0].p.y+=1.3*ship_scale;
	b.v[1].p.x-=1.8141*ship_scale;
	b.v[2].p.y-=1.3*ship_scale;

	b.v[0].p.x+=0.25;
	b.v[1].p.x+=0.20;
	b.v[2].p.x+=0.25;

	c = (struct vector) {1,0.6,0};
	b.v[0].c = c;
	b.v[1].c = c;
	b.v[2].c = c;

	b.v[0].n = normal(a.v[0].p);
	b.v[1].n = normal(a.v[1].p);
	b.v[2].n = normal(a.v[2].p);

	glBegin(GL_TRIANGLES);
	draw_poly(b);
	draw_poly(a);
	glEnd();
}

void draw_orbit_path()
{
	struct vector c = {0,1,1};

	struct vector a,b;
	a = (struct vector) {-10,-3,1};
	b = (struct vector) {-7,0,1};
	draw_line(a,b,c);

	a = b;
	b = (struct vector){-2,3,1};
	draw_line(a,b,c);

	a = b;
	b = (struct vector){3,3,1};
	draw_line(a,b,c);

	draw_selection_plane(b,c);

	a = b;
	b = (struct vector){7,0,1};
	draw_stippled_line(a,b,c);

	a = b;
	b = (struct vector){10,-3,1};
	draw_stippled_line(a,b,c);
}

double planet_rotation;
void draw_main_menu()
{
	GLfloat HUD_Color[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, HUD_Color);
	glPushMatrix();
	gluLookAt(0,0,0, 0,0,1, 0,1,0);

	draw_stars();

	planet_rotation+=0.10;
	draw_model(mm_planet_model,(struct vector){0,-12,0},
			planet_rotation, (struct vector){1.0,0,0.5});

	draw_orbit_path();

	glBegin(GL_TRIANGLES);
	draw_text(3.0,5,"BACK INTO ORBIT",(struct vector) {0,1,1});
	if(in_options)
		draw_ui(&options_ui);
	else
		draw_ui(&main_menu_ui);

	glEnd();

	glPopMatrix();
}

void scene_test()
{
	struct aiScene* scene = get_aiScene("models/animation_sample.dae");
	if(scene==NULL){
		printf("scene test err\n");
		return;
	}
	printf("Num animations:%i\n",scene->mNumAnimations);
	aiReleaseImport(scene);
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

	if(in_main_menu)
		draw_main_menu();
	else
		draw_game(gs);

	scene_test();

	SDL_GL_SwapWindow(window);
	process_events();
}
