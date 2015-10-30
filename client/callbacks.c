#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <input.h>

#include "callbacks.h"
#include "engine.h"
#include "game.h"
#include "graphics.h"

struct player_input pi;

void err_callback(int err, const char* description)
{
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods)
{
	if(key==256)/* 256 is ESC */
		exit(0);
	pi.keys[key]=action;//action is a binary (press||depress)
}

void cursor_callback(GLFWwindow * win, double xpos, double ypos)
{
	pi.mouse_x=xpos;
	pi.mouse_y=ypos;
}

//TODO actually calculate the right point here
void cursor_button_callback(GLFWwindow * win, int button, int action, int mods)
{
	//for button, 0 = left, 1 = right, 2 = middle
	if(button==0)
		pi.left_click=action;
	if(button==1)
		pi.right_click=action;
}
