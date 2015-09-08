#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include "callbacks.h"
#include "poly.h"
#include "graphics.h"
#include "engine.h"
#include "input.h"
#include "game.h"
#include "map.h"

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
	double ny = main_player.location.y+delta.y;
	double nz = main_player.location.z+delta.z;

	if(nz<0 || ny<0 || nz<0){
		printf("ERR: player outside of map");
		exit(0);
	}

	if(!world_map.tiles[(int)nx]
			[(int)main_player.location.z])
		main_player.location.x=nx;
	if(!world_map.tiles[(int)main_player.location.x]
			[(int)nz])
		main_player.location.z=nz;
	
}

void player_move(double dt)
{
#define player_speed 0.05
	struct vector delta;
	delta.x=0; delta.y=0; delta.z=0;

	if(main_player.location.x > main_player.destination.x)
		delta.x-=player_speed;
	if(main_player.location.x < main_player.destination.x)
		delta.x+=player_speed;

	if(main_player.location.z > main_player.destination.z)
		delta.z-=player_speed;
	if(main_player.location.z < main_player.destination.z)
		delta.z+=player_speed;

	do_move(delta);
}

void tick()
{
	double dt = delta_time();
	player_move(dt);
}
