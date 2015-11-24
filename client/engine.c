#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include <game_state.h>
#include <protolol.h>
#include <room.h>

#include "callbacks.h"
#include "networking.h"


/* all of the game engine stuff is actually server side */
void engine_tick(int server_fd, struct game_state * gs)
{
	*gs = update_state(server_fd,*gs,pi);
}
