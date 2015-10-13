#ifndef ENGINE
#define ENGINE

extern struct game_state gs;

void engine_tick();
void move(double x, double z);
void attack(double x, double z);
 
#endif
