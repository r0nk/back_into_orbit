#include <map.h>

#include "game_logic.h"
#include "client.h"

int near(struct vector a, struct vector b)
{
	if(( a.x > (b.x - 1)  && a.x < b.x + 1) &&
		(a.z > (b.z-1) && a.z < (b.z+1))){
			return 1;
	}
	return 0;
}

void bullet_update(struct game_state * gs, double delta)
{
	int i;
	for(i=0;i<gs->n_bullets;i++){
		gs->bullet[i].location.x+=
			(gs->bullet[i].direction.x*gs->bullet[i].speed)*delta;
		gs->bullet[i].location.z+=
			(gs->bullet[i].direction.z*gs->bullet[i].speed)*delta;
	}
}

void npc_update(struct game_state * gs,double delta)
{
	gs->n_npcs=1;
	gs->npc[0].location.x=5;
	gs->npc[0].location.y=0;
	gs->npc[0].location.z=5;

	if(gs->n_players > 0)
		gs->npc[0].location = gs->game_player[0].location;

	if(gs->npc[0].location.x>10 || gs->npc[0].location.x<0)
		gs->npc[0].location.x=5 + delta;
	if(gs->npc[0].location.z>10 || gs->npc[0].location.z<0)
		gs->npc[0].location.z=5 + delta;

	if((gs->npc[0].location.x ==  gs->game_player[0].location.x) &&
		 (gs->npc[0].location.z ==  gs->game_player[0].location.z)){
		gs->game_player[0].health-=0.100;
	}
}

void fire_bullet(struct vector starting, struct vector direction)
{
	struct bullet b;
	b.location = starting;
	b.direction = direction;
	b.speed = 5;
	b.duration = 100;
	add_bullet(&world_state,b);
}

void move_unit(struct unit * u,struct vector d)
{
	if(!world_map.tiles[ (int)(u->location.x+d.x) ][(int)(u->location.z)])
		u->location.x+=d.x;
	if(!world_map.tiles[ (int)(u->location.x)][(int)(u->location.z+d.z)])
		u->location.z+=d.z;
}

void player_movement(struct game_state * gs, double delta, int i)
{
	double d = gs->game_player[i].speed*delta;
	struct vector dvec = (struct vector){0,0,0};
	if(clients[i].pi.keys['W']){
		dvec.x-=d;
		dvec.z-=d;
	}
	if(clients[i].pi.keys['A']){
		dvec.x-=d;
		dvec.z+=d;
	}
	if(clients[i].pi.keys['S']){
		dvec.x+=d;
		dvec.z+=d;
	}
	if(clients[i].pi.keys['D']){
		dvec.x+=d;
		dvec.z-=d;
	}
	move_unit(&gs->game_player[i],dvec);
}

void player_attack(struct game_state * gs, double delta, int i)
{
//TODO temporary firing rate definition
#define FR 4
	if(gs->game_player[i].cooldown<1){
		if(clients[i].pi.keys['J']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {1,0,1});
			gs->game_player[i].cooldown = FR;
		}
		if(clients[i].pi.keys['H']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {-1,0,1});
			gs->game_player[i].cooldown = FR;
		}
		if(clients[i].pi.keys['K']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {-1,0,-1});
			gs->game_player[i].cooldown = FR;
		}
		if(clients[i].pi.keys['L']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {1,0,-1});
			gs->game_player[i].cooldown = FR;
		}
	} else {
		gs->game_player[i].cooldown-=10*delta;
	}
}

void player_controls(struct game_state * gs,double delta)
{
	int i = 0;
	for(i=0;i<n_clients;i++){
		player_movement(gs,delta,i);
		player_attack(gs,delta,i);
	}
}

void flag_update(struct game_state * gs,double delta)
{
	int i = 0;
	for(i=0;i<n_clients;i++){
		if(near(gs->game_player[i].location,gs->blue_flag.location)&&
				(gs->game_player[i].team==RED_TEAM)){
			gs->blue_flag.location=gs->game_player[i].location;
		}
		if(near(gs->game_player[i].location,gs->red_flag.location)&&
				(gs->game_player[i].team==BLUE_TEAM)){
			gs->red_flag.location=gs->game_player[i].location;
		}
	}
}

