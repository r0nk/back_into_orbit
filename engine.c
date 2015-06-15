#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include "graphics.h"
#include "engine.h"
#include "input.h"
#include "game.h"

double delta_time()
{
	double s = glfwGetTime();
	glfwSetTime(0.0);
	return s;
}

void player_move(double dt)
{
	double sci = 1/(fabs(camera.rot.z)+fabs(camera.rot.x));
#define player_speed 5
	if(keys['W']){
		main_player.location.z+= (sci*camera.rot.z) * dt * player_speed;
		main_player.location.x+= (sci*camera.rot.x) * dt * player_speed;
	}
	if(keys['A']){
		main_player.location.z-= camera.right.z * dt * player_speed;
		main_player.location.x-= camera.right.x * dt * player_speed;
	}
	if(keys['S']){
		main_player.location.z-= (sci*camera.rot.z) * dt * player_speed;
		main_player.location.x-= (sci*camera.rot.x) * dt * player_speed;
	}
	if(keys['D']){
		main_player.location.z+= camera.right.z * dt * player_speed;
		main_player.location.x+= camera.right.x * dt * player_speed;
	}
}

void tick()
{
	double dt = delta_time();
	player_move(dt);
	camera.eye.x = main_player.location.x;
	camera.eye.y = main_player.location.y+4.6;
	camera.eye.z = main_player.location.z;
}
