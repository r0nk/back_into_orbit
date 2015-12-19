#ifndef ENGINE
#define ENGINE
#include <math.h>

double fps;
long double frames;
long double seconds;

extern struct game_state gs;

void engine_tick();

static inline double to_degrees(double r){
	return r * (180.0/M_PI);
}

static inline double to_radians(double d){
	return d * (M_PI/180.0);
}
 
#endif
