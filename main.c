#include <unistd.h>
#include <stdio.h>

#include "game_state.h"
#include "graphics.h"
#include "engine.h"
#include "game.h"
#include "audio.h"
#include "map.h"

int main()
{
	init_graphics();
	init_audio();
	world_map=generate_map();
	world_map.current_room=&world_map.room[0];
	dump_map(&world_map);
	while(1){
		engine_tick(&(world_map.current_room->gs));
		graphics_draw(&(world_map.current_room->gs));
	}
	return 0;
}

