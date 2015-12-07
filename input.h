#ifndef INPUT
#define INPUT

#define MAX_KEYS 255

struct player_input {
	char left_click,right_click;
	double mouse_x,mouse_y;
	char keys[MAX_KEYS];
};

static inline void dump_player_input(struct player_input pi)
{
	printf("player_input:\n");
	printf(" left_click:%i, right click:%i\n",pi.left_click,pi.right_click);
	printf(" mouse {%f,%f}\n",pi.mouse_x,pi.mouse_y);
	printf("keys:\n");
	int i;
	for(i=0;i<MAX_KEYS;i++){
		if(pi.keys[i])
			printf("%c",i);
	}
	printf("\n");
}

struct player_input pi;
 
#endif
