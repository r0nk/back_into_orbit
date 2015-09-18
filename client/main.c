#include <unistd.h>
#include <stdio.h>
#include "graphics.h"
#include "engine.h"
#include "game.h"
#include "networking.h"

int main()
{
	init_graphics();
	int server_fd = init_networking();
	struct game_state gs;
	gs= init_game();
	while(1){
		engine_tick(server_fd,gs);
		graphics_draw();
	}
	return 0;
}

