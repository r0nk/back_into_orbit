#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "callbacks.h"
#include "engine.h"
#include "game.h"
#include "graphics.h"
#include "input.h"

char keys[NUMBER_OF_KEYS];/*boolean array of the input keys*/

void err_callback(int err, const char* description)
{
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods)
{
	if(key==256)/* ESC */
		exit(0);
	keys[key]=action;//action is a binary (press||depress)
}

void cursor_callback(GLFWwindow * win, double xpos, double ypos)
{
	cursor_x=xpos;
	cursor_y=ypos;
}

//TODO actually calculate the right point here
//TODO find a way to remove main_player struct
void cursor_button_callback(GLFWwindow * win, int button, int action, int mods)
{
	double w = window_width/2;
	double h = window_height/2;
	double cx = 7.5*((cursor_x-w)/w);
	double cy = 13.0*((cursor_y-h)/h);

	//for button, 0 = left, 1 = right, 2 = middle

	if(button==0){// left click
		move(cx,cy);
		/*
		 * main_player.destination.x=main_player.location.x+cx+cy;
		 * main_player.destination.z=main_player.location.z-cx+cy;
		*/
	}

	if(button==1 && action==1){// right click
		attack(cx,cy);
	}
}
