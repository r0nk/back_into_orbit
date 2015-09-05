#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "callbacks.h"
#include "game.h"
#include "graphics.h"
#include "input.h"

char keys[NUMBER_OF_KEYS];/*boolean array of the input keys*/

void err_callback(int err, const char* description){
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods){
	if(key==256)/* ESC */
		exit(0);
	keys[key]=action;
}

void cursor_callback(GLFWwindow * win, double xpos, double ypos)
{
	cursor_x=xpos;
	cursor_y=ypos;

	xpos*=-0.01;
	ypos*=0.01;

	camera.rot.x=cos(ypos) * sin(xpos);
	camera.rot.z=cos(ypos) * cos(xpos);
	camera.rot.y=sin(ypos);
	camera.right.x=sin(xpos - 3.14f/2.0f);
	camera.right.z=cos(xpos - 3.14f/2.0f);
}

void cursor_button_callback(GLFWwindow * win, int button, int action, int mods)
{
	//for button, 0 = left, 1 = right, 2 = middle
	main_player.destination.x=0;main_player.destination.z=0;

	main_player.destination.x = 
		main_player.location.x+(cursor_x-(window_width/2));
	main_player.destination.z = 
		main_player.location.z-(cursor_x-(window_width/2));

	main_player.destination.x +=
		main_player.location.x+(cursor_y-(window_height/2));
	main_player.destination.z +=
		main_player.location.z+(cursor_y-(window_height/2));


	printf("l: %f %f %f \n",main_player.location.x,main_player.location.y,main_player.location.z);
	printf("d: %f %f %f \n\n",main_player.destination.x,main_player.destination.y,main_player.destination.z);
}
