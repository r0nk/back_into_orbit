#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "callbacks.h"
#include "game.h"
#include "graphics.h"

void err_callback(int err, const char* description){
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods){
	if(!action)/*if the key is being released*/
		return;
	if(key=='W'){
		camera.eye.z+=camera.rot.z;
		camera.eye.x+=camera.rot.x;
	}
	if(key=='A'){
		camera.eye.z-=camera.right.z;
		camera.eye.x-=camera.right.x;
	}
	if(key=='S'){
		camera.eye.z-=camera.rot.z;
		camera.eye.x-=camera.rot.x;
	}
	if(key=='D'){
		camera.eye.z+=camera.right.z;
		camera.eye.x+=camera.right.x;
	}
	if(key==256)//ESC
		exit(0);
	//TODO
}

void cursor_callback(GLFWwindow * win, double xpos, double ypos)
{
	ypos*=-0.001;
	xpos*=-0.002;
	camera.rot.x=cos(ypos) * sin(xpos);
	camera.rot.y=sin(ypos);
	camera.rot.z=cos(ypos) * cos(xpos);
	camera.right.x=sin(xpos - 3.14f/2.0f);
	camera.right.z=cos(xpos - 3.14f/2.0f);
}

void cursor_button_callback(GLFWwindow * win, int button, int action, int mods)
{
	//for button, 0 = left, 1 = right, 2 = middle
	;

}
