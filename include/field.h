#ifndef FIELD
#define FIELD

#include "dimensions.h"
#include "poly.h"

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
