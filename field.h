#ifndef FIELD
#define FIELD

#include "room.h"

struct field
{
	/*for each cell vector:
	 * x = x velocity
	 * y = "pressure"
	 * z = z velocity
	 */
	struct vector cell[MAX_ROOM_WIDTH][MAX_ROOM_HEIGHT];
};
 
#endif
