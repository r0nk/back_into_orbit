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
#include "input.h"
#include "game.h"
#include "map.h"

struct player main_player;

/*get the time since the last delta_time() call*/
double delta_time()
{
	double s = glfwGetTime();
	glfwSetTime(0.0);
	return s;
}

/*try to actually move the player*/
void do_move(struct vector delta)
{
	double nx = main_player.location.x+delta.x;
	double nz = main_player.location.z+delta.z;

	if(!world_map.tiles[(int)nx]
			[(int)main_player.location.z])
		main_player.location.x=nx;
	if(!world_map.tiles[(int)main_player.location.x]
			[(int)nz])
		main_player.location.z=nz;
}

void player_move(double dt,struct game_state * gs)
{
	struct vector delta;
	delta.x=0; delta.y=0; delta.z=0;

	double a,o,h;
	a = main_player.destination.x - main_player.location.x;
	o = main_player.destination.z - main_player.location.z;
	h = sqrt((a*a)+(o*o));

	delta.x=(a/h) * main_player.speed * dt;
	delta.z=(o/h) * main_player.speed * dt;

	do_move(delta);
	gs->player_location[gs->current_player] = main_player.location;
}


void engine_tick(int server_fd, struct game_state * gs)
{
	double dt = delta_time();
	player_move(dt,gs);
	//TODO: other misc logic; enemies, etc. here
	*gs = update_state(server_fd,*gs);
}
