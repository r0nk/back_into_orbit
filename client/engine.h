#ifndef ENGINE
#define ENGINE

extern struct game_state gs;

void engine_tick();
void move(double x, double y);
void attack(double x, double y);
 
#endif
