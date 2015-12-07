#include <unistd.h>
#include <stdio.h>

#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "game.h"

struct game_state gs;

int main()
{
	init_graphics();
	gs = init_game();
	while(1){
		engine_tick(&gs);
		graphics_draw(&gs);
	}
	return 0;
}

