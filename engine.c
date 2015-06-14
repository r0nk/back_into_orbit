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
		camera.eye.z+= (sci*camera.rot.z) * dt * player_speed;
		camera.eye.x+= (sci*camera.rot.x) * dt * player_speed;
	}
	if(keys['A']){
		camera.eye.z-= camera.right.z * dt * player_speed;
		camera.eye.x-= camera.right.x * dt * player_speed;
	}
	if(keys['S']){
		camera.eye.z-= (sci*camera.rot.z) * dt * player_speed;
		camera.eye.x-= (sci*camera.rot.x) * dt * player_speed;
	}
	if(keys['D']){
		camera.eye.z+= camera.right.z * dt * player_speed;
		camera.eye.x+= camera.right.x * dt * player_speed;
	}
}

void tick()
{
	double dt = delta_time();
	player_move(dt);
}
