#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include "pathfinding.h"
#include "game_state.h"
#include "room.h"
#include "callbacks.h"
#include "audio.h"
#include "input.h"
#include "engine.h"
#include "map.h"
#include "effects.h"
#include "ui.h"

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
	return (distance(a,b)<r);
}

/*copy-pasted from the webs*/
int line_intersects_line(struct vector a, struct vector b,
		struct vector c, struct vector d)
{
	double denominator=((b.x - a.x)*(d.z - c.z))-((b.z - a.z)*(d.x - c.x));
	double numerator1=((a.z - c.z)*(d.x - c.x))-((a.x - c.x)*(d.z - c.z));
	double numerator2=((a.z - c.z)*(b.x - a.x))-((a.x - c.x)*(b.z - a.z));

	// Detect coincident lines (has a problem, read below)
	if (denominator == 0) 
		return (numerator1 == 0 && numerator2 == 0);

	double r = numerator1 / denominator;
	double s = numerator2 / denominator;

	return ((r >= 0 && r <= 1) && (s >= 0 && s <= 1));
}


int unit_intersects_line(struct unit u, struct vector a, struct vector b)
{
	/*
	   h     j
	   +-----+
	   |  .  |
	   +-----+
	   k     l
	 */
	struct vector h=u.location,j=u.location,k=u.location,l=u.location;
	h.x-=u.hit_radius;
	j.x+=u.hit_radius;
	k.x-=u.hit_radius;
	l.x+=u.hit_radius;

	h.z+=u.hit_radius;
	j.z+=u.hit_radius;
	k.z-=u.hit_radius;
	l.z-=u.hit_radius;

	if(line_intersects_line(a,b,h,j))
		return 1;
	if(line_intersects_line(a,b,h,k))
		return 1;
	if(line_intersects_line(a,b,k,l))
		return 1;
	if(line_intersects_line(a,b,j,l))
		return 1;

	return 0;
}

int line_intersects_box(struct vector boxloc,struct vector a, struct vector b)
{
	/*
	   h     j
	   +-----+
	   |  .  |
	   +-----+
	   k     l
	 */
	struct vector h=boxloc,j=boxloc,k=boxloc,l=boxloc;
	j.x+=1.0;
	l.x+=1.0;

	k.z+=1.0;
	l.z+=1.0;

	if(line_intersects_line(a,b,h,j))
		return 1;
	if(line_intersects_line(a,b,h,k))
		return 1;
	if(line_intersects_line(a,b,k,l))
		return 1;
	if(line_intersects_line(a,b,j,l))
		return 1;

	return 0;
}

int wall_intersects_line(struct vector a, struct vector b)
{
	struct layout l = world_map.current_room->layout;
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			if(l.tiles[i][j]!='#')
				continue;
			if(line_intersects_box((struct vector) {i,0,j},a,b))
				return 1;
		}
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
		if(!world_map.current_room->doorway[i].is_connected){
			return -1;
		}
		if(near(world_map.current_room->doorway[i].location,l,1)){
			return world_map.current_room->doorway[i].index;
		}
	}
	return -1;
}

void aoe_damage(struct game_state * gs,struct vector location,
		int amount, int radius)
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

		if(near(gs->bullet[i].location,gs->game_player.location,1.0)){
			gs->game_player.health-=gs->bullet[i].damage;
			tzztzzz();
			remove_bullet(gs,i);
		}
		for(j=0;j<gs->n_npcs;j++){
			if(near(gs->bullet[i].location,
						gs->npc[j].location,
						gs->npc[j].hit_radius)){
				gs->npc[j].health-=gs->bullet[i].damage;
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
	b.damage = u.damage;
	b.flags=u.flags;
	pew();
	add_bullet(gs,b);
}

void move_unit(struct unit * u,struct vector d)
{
	struct vector vec;
	if(isnan(d.x) || isnan(d.z)){
		printf("NAN d passed to move_unit()\n");
		return;
	}

	vec.x = (d.x>0) ? 1 : -1;
	vec.z = (d.z>0) ? 1 : -1;
	d.x*=u->speed;
	d.z*=u->speed;
	struct layout  * l = &(world_map.current_room->layout);
	if((u->location.x+d.x)>MAX_ROOM_WIDTH)
		return;
	if((u->location.z+d.z)>MAX_ROOM_HEIGHT)
		return;
	if((u->location.x+d.x)<0)
		return;
	if((u->location.z+d.z)<0)
		return;
	if((l->tiles[(int)(u->location.x+d.x)][(int)(u->location.z)]!= LAYOUT_WALL)){
		u->location.x+=d.x;
	}

	if((l->tiles[(int)(u->location.x)][(int)(u->location.z+d.z)]!= LAYOUT_WALL)){
		u->location.z+=d.z;
	}

	if(!l->tiles[(int)(u->location.x)][(int)(u->location.z)])
		u->health=0;/* oops, they went out of the map */
}

void player_movement(struct game_state * gs, double delta)
{
	struct vector dvec = (struct vector){0,0,0};

	if(gs->game_player.dash_timer>0){

		dvec.x=sin(to_radians(gs->game_player.rotation_angle))*delta*11;
		dvec.z=cos(to_radians(gs->game_player.rotation_angle))*delta*11;

		move_unit(&gs->game_player,dvec);
		gs->game_player.dash_timer-=delta;
		return;
	}

	if(pi.keys['W']){
		dvec.x-=delta;
		dvec.z-=delta;
	}
	if(pi.keys['A']){
		dvec.x-=delta;
		dvec.z+=delta;
	}
	if(pi.keys['S']){
		dvec.x+=delta;
		dvec.z+=delta;
	}
	if(pi.keys['D']){
		dvec.x+=delta;
		dvec.z-=delta;
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
				gs->game_player.damage=10;
			}
		}else{
			fire_bullet(gs,gs->game_player,v);
			gs->game_player.cooldown = 0.5;
			gs->game_player.damage=10;
		}
	}else{
		player_fired=0;
	}
}

void game_over(struct game_state * gs)
{
	deathplosion();
	sleep(1);/* give them time to look at their failures >:( */
	ui=&gameover_ui;
	paused=1;
	is_game_over=1;
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
	if(gs->game_player.health<=0){
		gameover_ui = gameover_menu(gs->game_player.score,0);
		game_over(gs);
	}
	player_movement(gs,delta);
	player_attack(gs,delta);
	player_items(gs,delta);
	door_check(gs);

	face(&gs->game_player,screen_to_world(gs,pi.mouse_x,pi.mouse_y));
}

void move_towards_facing(struct unit * u,double delta)
{
	struct vector v = (struct vector) {0,0,0};

	v.x=sin(to_radians(u->rotation_angle))*delta;
	v.y=0;
	v.z=cos(to_radians(u->rotation_angle))*delta;

	move_unit(u,v);
}

void update_scavenger(struct game_state * gs, double delta, int j)
{
	if(gs->npc[j].path_timer>0){
		move_towards_facing(&gs->npc[j],delta);
		gs->npc[j].path_timer-=delta;
	}
	if(near(gs->game_player.location,gs->npc[j].location,1.5)){
		gs->game_player.health-=
			delta*gs->npc[j].damage*gs->game_player.resist;
		tzztzzz();
	} else if(near(gs->game_player.location,gs->npc[j].location,15)){
		if(wall_intersects_line(gs->game_player.location,
					gs->npc[j].location)){
			gs->npc[j].path = pathfind(gs->npc[j].location, 
					gs->game_player.location);
			struct vector d = path_pop(&gs->npc[j].path);
			d=path_pop(&gs->npc[j].path);
			gs->npc[j].path_timer=1;

			face(&gs->npc[j],d);
		}else{
			face(&gs->npc[j],gs->game_player.location);
		}
		move_towards_facing(&gs->npc[j],delta);
	}

	if(gs->npc[j].poison_timer>0.0){
		gs->npc[j].health-=delta*3;
		gs->npc[j].poison_timer-=delta;
	}
}

void update_item_npc(struct game_state * gs, double delta, int j)
{
	gs->npc[j].rotation_angle += 45*delta;
	if(near(gs->game_player.location, gs->npc[j].location,1.5)){
		gs->game_player.score+=gs->npc[j].score;
		remove_npc(gs,j);
		add_item(&(gs->game_player.inventory),coin_item());
	} 	
}

void update_boss(struct game_state * gs, double delta, int j)
{
	struct unit npc;
	struct vector loc;

	gs->npc[j].rotation_angle+=delta*180;
	if(gs->npc[j].cooldown>0){
		gs->npc[j].cooldown-=delta;
		return;
	}
	loc = gs->npc[j].location;
	loc.x+=(rand()%4)-2;
	loc.z+=(rand()%4)-2;
	gs->npc[j].cooldown=2;
	npc = scavenger_npc(loc);
	add_npc(gs,npc);
}

void death(struct game_state * gs, int j)
{
	deathplosion();
	gs->game_player.score+=gs->npc[j].score;
	if(!(rand()%2)){
		add_npc(gs,item_npc(gs->npc[j].location,ITEM_COIN));
	}
	if(gs->npc[j].type == UNIT_TYPE_BOSS){
		gameover_ui = gameover_menu(gs->game_player.score,1);
		game_over(gs);
	}
	if(gs->npc[j].type == UNIT_TYPE_YO){
		remove_npc(gs,j);
		remove_npc(gs,gs->npc[j].connected_to);
		gameover_ui = gameover_menu(gs->game_player.score,1);
		game_over(gs);
	}
	remove_npc(gs,j);
}

void update_ranger(struct game_state * gs, double delta, int j)
{
	struct vector d;
	d.x=sin(to_radians(gs->npc[j].rotation_angle));
	d.y=0.0;
	d.z=cos(to_radians(gs->npc[j].rotation_angle));

	if(gs->npc[j].poison_timer>0.0){
		gs->npc[j].health-=delta*3;
		gs->npc[j].poison_timer-=delta;
	}

	if(gs->npc[j].path_timer>0){
		move_towards_facing(&gs->npc[j],delta);
		gs->npc[j].path_timer-=delta;
	}

	if(near(gs->game_player.location,gs->npc[j].location,7.5) 
			&&! wall_intersects_line(gs->game_player.location,
				gs->npc[j].location)){
		face(&gs->npc[j],gs->game_player.location);
		if(gs->npc[j].cooldown>0){
			gs->npc[j].cooldown-=delta;
		}else{
			fire_bullet(gs,gs->npc[j],d);
			gs->npc[j].cooldown=1;
		}
	} else if(near(gs->game_player.location,gs->npc[j].location,15)){
		if(wall_intersects_line(gs->game_player.location,
					gs->npc[j].location)){
			gs->npc[j].path = pathfind(gs->npc[j].location, 
					gs->game_player.location);
			struct vector d = path_pop(&gs->npc[j].path);
			d=path_pop(&gs->npc[j].path);
			gs->npc[j].path_timer=1;

			face(&gs->npc[j],d);
		}else{
			face(&gs->npc[j],gs->game_player.location);
		}
		move_towards_facing(&gs->npc[j],delta);
	}
}

void teleport_mole(struct unit * mole)
{
	switch(rand()%4)
	{
		case 0:
			mole->location = (struct vector) {5,0,5};
			break;
		case 1:
			mole->location = (struct vector) {5,0,10};
			break;
		case 2:
			mole->location = (struct vector) {10,0,5};
			break;
		case 3:
			mole->location = (struct vector) {10,0,10};
			break;
	}
	mole->cooldown2 = 2;
}

void update_mole(struct game_state * gs, double delta,int j)
{
	struct vector d;
	d.x=sin(to_radians(gs->npc[j].rotation_angle));
	d.y=0.0;
	d.z=cos(to_radians(gs->npc[j].rotation_angle));

	if(gs->npc[j].cooldown2 > 0){
		gs->npc[j].cooldown2-=delta;
	}else{
		teleport_mole(&gs->npc[j]);
	}

	if(near(gs->game_player.location,gs->npc[j].location,7.5)){
		face(&gs->npc[j],gs->game_player.location);
		if(gs->npc[j].cooldown>0){
			gs->npc[j].cooldown-=delta;
		}else{
			fire_bullet(gs,gs->npc[j],d);
			gs->npc[j].cooldown=0.4;
		}
	} 
	if(gs->npc[j].poison_timer>0.0){
		gs->npc[j].health-=delta*3;
		gs->npc[j].poison_timer-=delta;
	}
}

void update_yo(struct game_state * gs, double delta, int j)
{
	/* wheter or not we're the (top_left|bottom_right) one */
	int w = (j < (gs->npc[j].connected_to))? 1 : 0;

	struct vector a = gs->npc[j].location;
	struct vector b = gs->npc[gs->npc[j].connected_to].location;
	if(unit_intersects_line(gs->game_player,a,b)){
		gs->game_player.health-=
			delta*gs->npc[j].damage*gs->game_player.resist;
		tzztzzz();
	}

	gs->npc[j].speed = 5+((100-(gs->npc[j].health))/4);

	/*
	   i---o
	   |   |
	   |   |
	   k---l
	 */
	int i = (b.x==1 && b.z==1);
	int o = (b.x==11 && b.z==1);
	int k = (b.x==1 && b.z==11);
	int l = (b.x==11 && b.z==11);

	double d = gs->npc[j].speed * delta;

	if(w){
		if(o){
			if(near(gs->npc[j].location,(struct vector) {11,0,11},1)){
				gs->npc[j].location=(struct vector) {11,0,11};
				return;
			}

			gs->npc[j].location.x+=d;
			gs->npc[j].location.z+=d;
		}else if (k){
			if(near(gs->npc[j].location,(struct vector) {1,0,1},1)){
				gs->npc[j].location=(struct vector) {1,0,1};
				return;
			}
			gs->npc[j].location.x-=d;
			gs->npc[j].location.z-=d;
		}
	}else{
		if(i){
			if(near(gs->npc[j].location,(struct vector) {11,0,1},1)){
				gs->npc[j].location=(struct vector) {11,0,1};
				return;
			}
			gs->npc[j].location.x+=d;
			gs->npc[j].location.z-=d;
		}else if (l){
			if(near(gs->npc[j].location,(struct vector) {1,0,11},1)){
				gs->npc[j].location=(struct vector) {1,0,11};
				return;
			}
			gs->npc[j].location.x-=d;
			gs->npc[j].location.z+=d;
		}
	}
}

void update_npcs(struct game_state * gs, double delta)
{
	int j;
	for(j=0;j<gs->n_npcs;j++){
		if(gs->npc[j].health<0){
			death(gs,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_YO){
			update_yo(gs,delta,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_NEUTRAL_CREEP){
			update_scavenger(gs,delta,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_MOLE){
			update_mole(gs,delta,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_RANGER){
			update_ranger(gs,delta,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_ITEM){
			update_item_npc(gs,delta,j);
		}

		if(gs->npc[j].type == UNIT_TYPE_BOSS){
			update_boss(gs,delta,j);
		}
	}
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

void update_shop(struct game_state * gs, double delta)
{
	struct shop * s = &(world_map.current_room->shop);
	int i;
	for(i=0;i<MAX_TRANSACTIONS;i++){
		if(near(gs->game_player.location,s->t[i].location,1)){
			if(!(s->t[i].sold) && 
			     spend(&gs->game_player.inventory,s->t[i].price)){
				s->t[i].sold=1;
				add_item(&gs->game_player.inventory,s->t[i].item);
			}
		}
	}
}

void engine_tick(struct game_state * gs)
{
	double d = delta_time();
	if(!paused && !in_main_menu){
		update_player(gs,d);
		update_bullets(gs,d);
		update_npcs(gs,d);
		if(world_map.current_room->has_shop){
			update_shop(gs,d);
		}
	}else{
		if(in_main_menu)
			update_ui(&main_menu_ui);
		else
			update_ui(ui);
	}
	count_fps(d);
}
