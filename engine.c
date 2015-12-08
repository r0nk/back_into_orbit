#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include "game_state.h"
#include "room.h"
#include "callbacks.h"
#include "input.h"

double delta_time()
{
	double s = glfwGetTime();
	glfwSetTime(0.0);
	return s;
}

void update_bullets(struct game_state * gs, double delta)
{
	int i;
	double s;
	for(i=0;i<gs->n_bullets;i++){
		s = gs->bullet[i].speed*delta;
		gs->bullet[i].location.x+=
			gs->bullet[i].direction.x * s;
		gs->bullet[i].location.z+=
			gs->bullet[i].direction.z * s;
		if(world_room.tiles[(int)gs->bullet[i].location.x]
				[(int)gs->bullet[i].location.z])
			remove_bullet(gs,i);
	}
}

void fire_bullet(struct game_state * gs,
		struct vector starting, struct vector direction)
{
	struct bullet b;
	b.location.x = starting.x+direction.x;
	b.location.y = starting.y+direction.y;
	b.location.z = starting.z+direction.z;
	b.direction = direction;
	b.speed = 5;
	b.duration = 100;
	add_bullet(gs,b);
}

void move_unit(struct unit * u,struct vector d)
{
	if(world_room.tiles[ (int)(u->location.x+d.x) ][(int)(u->location.z)]
			!= ROOM_WALL)
		u->location.x+=d.x;
	if(world_room.tiles[ (int)(u->location.x)][(int)(u->location.z+d.z)]
			!= ROOM_WALL)
		u->location.z+=d.z;
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

void player_attack(struct game_state * gs, double delta)
{
#define FR 4
	if(gs->game_player.cooldown<1){
		if(pi.keys['J']){
			fire_bullet(gs,gs->game_player.location,
					(struct vector) {1,0,1});
			gs->game_player.cooldown = FR;
		}
		if(pi.keys['H']){
			fire_bullet(gs,gs->game_player.location,
					(struct vector) {-1,0,1});
			gs->game_player.cooldown = FR;
		}
		if(pi.keys['K']){
			fire_bullet(gs,gs->game_player.location,
					(struct vector) {-1,0,-1});
			gs->game_player.cooldown = FR;
		}
		if(pi.keys['L']){
			fire_bullet(gs,gs->game_player.location,
					(struct vector) {1,0,-1});
			gs->game_player.cooldown = FR;
		}
	} else {
		gs->game_player.cooldown-=10*delta;
	}
}

void update_player(struct game_state * gs,double delta)
{
	player_movement(gs,delta);
	player_attack(gs,delta);
}

/* all of the game engine stuff is actually server side */
void engine_tick(struct game_state * gs)
{
	double d = delta_time();
	update_player(gs,d);
	update_bullets(gs,d);
}
