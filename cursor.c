#include "graphics.h"
#include "cursor.h"

struct vector pixel_to_view(int x, int y)
{
	struct vector ret;

	y = window_height-y;

	ret.x = x - (window_width/2);
	ret.y = y - (window_height/2);

	ret.x /= (10*(window_width));
	ret.y /= (10*(window_height));
	
	ret.x*=20;
	ret.y*=20;

	ret.x*=10;
	ret.y*=10;

	return ret;
}
