#ifndef GRAPHICS
#define GPRAHICS 1

#include "poly.h"

extern int window_width,window_height;

int init_graphics();
void graphics_draw();
void draw_letter(double x, double y, char bits[24]);
void deinit_graphics();

#endif
