#ifndef INPUT
#define INPUT

#define MAX_KEYS 255

struct player_input {
	char left_click,right_click;
	double mouse_x,mouse_y;
	char keys[MAX_KEYS];
};
 
#endif
