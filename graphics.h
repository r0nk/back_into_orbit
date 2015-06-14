#ifndef GRAPHICS
#define GPRAHICS 1

#include "poly.h"

struct camera {
	struct vector eye;
	struct vector rot;
	struct vector up;
	struct vector right;
};

extern struct camera camera;

int init_graphics();
void draw();
void deinit_graphics();

#endif
