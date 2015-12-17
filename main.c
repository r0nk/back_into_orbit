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

int main()
{
	srand(time(NULL));
	init_graphics();
	init_audio();
	world_map=generate_map();
	world_map.current_room=&world_map.room[0];
//	ProfilerStart("profdata");
	while(1){
		engine_tick(&(world_map.current_room->gs));
		graphics_draw(&(world_map.current_room->gs));
		if(pi.keys[256])
			break;
	}
//	ProfilerStop();
	return 0;
}

