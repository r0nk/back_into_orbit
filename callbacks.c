#include "callbacks.h"
#include "stdio.h"

void err_callback(int err, const char* description){
	fputs(description,stderr);
}

void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods){
	printf("keypress: %i",key);
	//TODO
}

