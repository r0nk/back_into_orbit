#ifndef INSTANCE
#define INSTANCE

#include <room.h>
#include <game_state.h>

struct instance
{
	struct room room;
	struct game_state state;
};
 
#endif
