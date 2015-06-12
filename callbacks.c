#include <stdio.h>
#include <stdlib.h>

#include "callbacks.h"
#include "game.h"

void err_callback(int err, const char* description){
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods){
	if(!action)/*if the key is being released*/
		return;
	if(key=='W')
		p.location.y++;
	if(key=='A')
		p.location.x--;
	if(key=='S')
		p.location.y--;
	if(key=='D')
		p.location.x++;
	if(key==256)//ESC
		exit(0);
	//TODO
}

