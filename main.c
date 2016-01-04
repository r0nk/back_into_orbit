#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <gperftools/profiler.h>

#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "game.h"
#include "audio.h"
#include "map.h"
#include "input.h"
#include "ui.h"

int main()
{
	srand(time(NULL));
	init_graphics();
	init_audio();
	generate_map(&world_map,1);
	init_ui();
	paused=0;
	is_game_over=0;
//	ProfilerStart("profdata");
	while(1){
		engine_tick(&(world_map.current_room->gs));
		graphics_draw(&(world_map.current_room->gs));
	}
//	ProfilerStop();
	return 0;
}
