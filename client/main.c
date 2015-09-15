#include <unistd.h>
#include <stdio.h>
#include "graphics.h"
#include "engine.h"
#include "game.h"

int main(){
	init_graphics();
	init_game();
	//init_networking(); TODO
	while(1){
		engine_tick();
		graphics_draw();
	}
	return 0;
}

