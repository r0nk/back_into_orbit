#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include <game_state.h>
#include <protolol.h>

#include "networking.h"
#include "callbacks.h"
#include "poly.h"
#include "graphics.h"
#include "engine.h"
#include "game.h"
#include "map.h"

void move(double x, double y)
{
	gs.game_player[gs.current_player].destination.x=
		gs.game_player[gs.current_player].location.x+x+y;
	gs.game_player[gs.current_player].destination.z=
		gs.game_player[gs.current_player].location.z+y-x;
}

void attack(double x, double y)
{
	struct bullet b;
	b.location = gs.game_player[gs.current_player].location;
	b.direction.x = x+y;
	b.direction.y = 0; 
	b.direction.z = y-x;
	printf("attack %f,%f\n",x,y);
	add_bullet(&gs,b);
}

/*get the time since the last delta_time() call*/
double delta_time()
{
	double s = glfwGetTime();
	glfwSetTime(0.0);
	return s;
}

/*try to actually move the player*/
void do_move(struct vector delta,struct game_state * gs)
{
	double nx = gs->game_player[gs->current_player].location.x+delta.x;
	double nz = gs->game_player[gs->current_player].location.z+delta.z;

	if(!world_map.tiles[(int)nx]
			[(int)gs->game_player[gs->current_player].location.z])
		gs->game_player[gs->current_player].location.x=nx;
	if(!world_map.tiles[(int)gs->game_player[gs->current_player].location.x]
			[(int)nz])
		gs->game_player[gs->current_player].location.z=nz;
}

void player_move(double dt,struct game_state * gs)
{
	struct vector delta;
	delta.x=0; delta.y=0; delta.z=0;

	double a,o,h;
	a = gs->game_player[gs->current_player].destination.x 
		- gs->game_player[gs->current_player].location.x;
	o = gs->game_player[gs->current_player].destination.z 
		- gs->game_player[gs->current_player].location.z;
	h = sqrt((a*a)+(o*o));

	delta.x=(a/h) * gs->game_player[gs->current_player].speed * dt;
	delta.z=(o/h) * gs->game_player[gs->current_player].speed * dt;

	do_move(delta,gs);
}

void engine_tick(int server_fd, struct game_state * gs)
{
	double dt = delta_time();
	player_move(dt,gs);
	*gs = update_state(server_fd,*gs);
}
