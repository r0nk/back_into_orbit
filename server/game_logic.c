#include "game_logic.h"
#include "client.h"

struct game_state world_state;

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

void player_movement(struct game_state * gs, double delta, int i)
{
	double d = gs->game_player[i].speed*delta;
	if(clients[i].pi.keys['W']){
		gs->game_player[i].location.x-=d;
		gs->game_player[i].location.z-=d;
	}
	if(clients[i].pi.keys['A']){
		gs->game_player[i].location.x-=d;
		gs->game_player[i].location.z+=d;
	}
	if(clients[i].pi.keys['S']){
		gs->game_player[i].location.x+=d;
		gs->game_player[i].location.z+=d;
	}
	if(clients[i].pi.keys['D']){
		gs->game_player[i].location.x+=d;
		gs->game_player[i].location.z-=d;
	}
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
