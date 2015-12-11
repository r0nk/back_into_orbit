#include "room.h"

struct room generate_room()
{
	struct room room;
	room.layout=get_layout("layouts/simple.layout");
	return room;
}
