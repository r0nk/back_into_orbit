#ifndef ENGINE
#define ENGINE

extern struct game_state gs;

void engine_tick();

static inline double to_degrees(double r){
	return r * (180.0/M_PI);
}
static inline double to_radians(double d){
	return d * (M_PI/180.0);
}
 
#endif
