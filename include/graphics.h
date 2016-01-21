#ifndef GRAPHICS
#define GPRAHICS 1

#include "poly.h"


extern int window_width,window_height;
extern float frame_x,frame_y;

int init_graphics();
void graphics_draw();
void draw_letter(double x, double y, char bits[24],struct vector color);
void deinit_graphics();

#endif
