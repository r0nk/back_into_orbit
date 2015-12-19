#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include "game_state.h"
#include "room.h"
#include "callbacks.h"
#include "audio.h"
#include "input.h"
#include "engine.h"
#include "map.h"
#include "effects.h"

double spawner_countdown;
#define SCM 10

void face(struct unit * u, point p)
{
	struct vector d;
	d.x = u->location.x - p.x;
	d.z = u->location.z - p.z;

	if(d.x<0.0)
		u->rotation_angle = to_degrees(-atan(d.z/d.x)) + 90;
	else
		u->rotation_angle = to_degrees(-atan(d.z/d.x)) - 90;
	u->rotation = (struct vector) {0,1,0};
}

int near(struct vector a, struct vector b,double r)
{
	if(( a.x > (b.x - r)  && a.x < b.x + r) &&
			(a.z > (b.z-r) && a.z < (b.z+r))){
		return 1;
	}
	return 0;
}

double delta_time()
{
	double s = glfwGetTime();
	glfwSetTime(0.0);
	return s;
}

int door_at(struct vector l)
{
	int i;
	for(i=0;i<world_map.current_room->n_doorways;i++){
		if(near(world_map.current_room->doorway[i].location,l,1)){
			return world_map.current_room->doorway[i].index;
		}
	}
	return -1;
}

void aoe_damage(struct game_state * gs,struct vector location,int amount, int radius)
{
	int i;
	for(i=0;i<gs->n_npcs;i++){
		if(near(gs->npc[i].location,location,radius)){
			gs->npc[i].health-=amount;
		}
	}
}

void update_bullets(struct game_state * gs, double delta)
{
	int i,j;
	double s;
	for(i=0;i<gs->n_bullets;i++){
		s = gs->bullet[i].speed*delta;
		gs->bullet[i].location.x+=gs->bullet[i].direction.x*s;
		gs->bullet[i].location.z+=gs->bullet[i].direction.z*s;
		if(world_map.current_room->layout.tiles[(int)gs->bullet[i].location.x]
				[(int)gs->bullet[i].location.z] == LAYOUT_WALL)
			remove_bullet(gs,i);

		for(j=0;j<gs->n_npcs;j++){
			if(near(gs->bullet[i].location,
						gs->npc[j].location,1.5)){
				gs->npc[j].health-=10;
				tzztzzz();
				if(gs->bullet[i].flags&HAS_VAIL)
					gs->npc[j].poison_timer+=5;
				if(gs->bullet[i].flags&HAS_VECTOR_FIELD)
					aoe_damage(gs,gs->bullet[i].location,5,3);
				remove_bullet(gs,i);
			}
		}
	}
}

void fire_bullet(struct game_state * gs,
		struct unit u, struct vector direction)
{
	struct bullet b;
	b.location.x = u.location.x+direction.x;
	b.location.y = u.location.y+direction.y;
	b.location.z = u.location.z+direction.z;
	b.direction = direction;
	b.speed = 10;
	b.duration = 100;
	b.flags=u.flags;
	pew();
	add_bullet(gs,b);
}

void move_unit(struct unit * u,struct vector d)
{
	if(world_map.current_room->layout.tiles[(int)(u->location.x+d.x)][(int)(u->location.z)]
			!= LAYOUT_WALL)
		u->location.x+=d.x;
	if(world_map.current_room->layout.tiles[(int)(u->location.x)][(int)(u->location.z+d.z)]
			!= LAYOUT_WALL)
		u->location.z+=d.z;
	if(!world_map.current_room->layout.tiles[(int)(u->location.x)][(int)(u->location.z)])
		u->health=0;/* oops, they wen't out of the map */
}

void player_movement(struct game_state * gs, double delta)
{
	double d = gs->game_player.speed*delta;
	struct vector dvec = (struct vector){0,0,0};


	if(pi.keys['W']){
		dvec.x-=d;
		dvec.z-=d;
	}
	if(pi.keys['A']){
		dvec.x-=d;
		dvec.z+=d;
	}
	if(pi.keys['S']){
		dvec.x+=d;
		dvec.z+=d;
	}
	if(pi.keys['D']){
		dvec.x+=d;
		dvec.z-=d;
	}

	move_unit(&gs->game_player,dvec);
}

int player_fired;

void player_attack(struct game_state * gs, double delta)
{
	struct vector v;
	v.x=sin(to_radians(gs->game_player.rotation_angle));
	v.y=0.0;
	v.z=cos(to_radians(gs->game_player.rotation_angle));
	if(gs->game_player.cooldown>0){
		gs->game_player.cooldown-=delta;
		return;
	}
	if(pi.left_click){
		if(gs->game_player.flags&HAS_TRIGGER){
			if(!player_fired){
				fire_bullet(gs,gs->game_player,v);
				player_fired=1;
				gs->game_player.cooldown = 0.0;
			}
		}else{
			fire_bullet(gs,gs->game_player,v);
			gs->game_player.cooldown = 0.5;
		}
	}else{
		player_fired=0;
	}
}

void game_over()
{
	printf("MISSON FAILED; B0T DESTORYED\n");
	deathplosion();
	sleep(1);/* give them time to look at their failures >:( */
	exit(0);
}

void player_items(struct game_state * gs, double delta)
{

	gs->game_player.inventory.item[0].active=pi.keys['1'];
	gs->game_player.inventory.item[1].active=pi.keys['2'];
	gs->game_player.inventory.item[2].active=pi.keys['3'];
	gs->game_player.inventory.item[3].active=pi.keys['4'];

	int i;
	for(i=0;i<gs->game_player.inventory.n_items;i++){
		item_effect(gs,&(gs->game_player.inventory.item[i]),delta);
	}
}

void door_check(struct game_state * gs)
{
	int t;
	t = door_at(gs->game_player.location);
	if(t!=-1){
		move_through_doorway(&world_map,t);
	}
}

void update_player(struct game_state * gs,double delta)
{
	if(gs->game_player.health<=0)
		game_over();
	player_movement(gs,delta);
	player_attack(gs,delta);
	player_items(gs,delta);
	door_check(gs);

	face(&gs->game_player,screen_to_world(gs,pi.mouse_x,pi.mouse_y));
}

void update_scavenger(struct game_state * gs, double delta, int j)
{
	if(near(gs->game_player.location,gs->npc[j].location,1.5)){
		gs->game_player.health-=delta*30;
		tzztzzz();
	} else if(near(gs->game_player.location,gs->npc[j].location,10)){
		face(&gs->npc[j],gs->game_player.location);
		struct vector v = (struct vector) {0,0,0};
		double s = gs->npc[j].speed*delta;

		v.x=sin(to_radians(gs->npc[j].rotation_angle))*s;
		v.z=cos(to_radians(gs->npc[j].rotation_angle))*s;

		move_unit(&gs->npc[j],v);
	}
	if(gs->npc[j].poison_timer>0.0){
		gs->npc[j].health-=delta*3;
		gs->npc[j].poison_timer-=delta;
	}
}

void update_item_npc(struct game_state * gs, double delta, int j)
{
	if(near(gs->game_player.location, gs->npc[j].location,1.5)){
		remove_npc(gs,j);
		add_item(&(gs->game_player.inventory),regen_item());
	} 	
}

void update_npcs(struct game_state * gs, double delta)
{
	int j;
	for(j=0;j<gs->n_npcs;j++){
		if(gs->npc[j].health<0){
			deathplosion();
			remove_npc(gs,j);
		}
		if(gs->npc[j].type == UNIT_TYPE_NEUTRAL_CREEP){
			update_scavenger(gs,delta,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_ITEM){
			update_item_npc(gs,delta,j);
		}
	}
}

void spawner (struct game_state * gs, double delta)
{
	struct unit npc;

	if(spawner_countdown>0){
		spawner_countdown-=delta;
		return;
	}
	spawner_countdown=SCM;
	npc.speed=2.0;
	npc.health=100;
	npc.location=(struct vector) {15 + delta*10,0,3};
	npc.destination=(struct vector) {15,0,3};
	npc.type = UNIT_TYPE_NEUTRAL_CREEP;
	npc.rotation_angle = 90;
	npc.rotation = (struct vector) {0,1,0};
	add_npc(gs,npc);
}

void count_fps(double d)
{
	frames++;
	seconds+=d;
	fps=frames/seconds;
	if(seconds>5){
		frames=1;
		seconds=d;
	}
}

/* all of the game engine stuff is actually server side */
void engine_tick(struct game_state * gs)
{
	double d = delta_time();
	//spawner(gs,d);
	update_player(gs,d);
	update_bullets(gs,d);
	update_npcs(gs,d);
	count_fps(d);
}
