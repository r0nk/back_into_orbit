#include <unistd.h>
#include <stdio.h>

#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "game.h"
#include "audio.h"
#include "map.h"

struct game_state gs;

int main()
{
	init_graphics();
	init_audio();
	gs = init_game();
	world_map=generate_map();
	world_map.current_room=&world_map.room[0];
	while(1){
		engine_tick(&gs);
		graphics_draw(&gs);
	}
	return 0;
}

