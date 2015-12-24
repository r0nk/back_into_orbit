#ifndef INPUT
#define INPUT

#include <stdio.h>
#include "graphics.h"
#include "poly.h"
#include "map.h"

#define MAX_KEYS 500

struct player_input {
	char left_click,right_click;
	double mouse_x,mouse_y;
	char keys[MAX_KEYS];
};

struct player_input pi;

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

static inline struct vector screen_to_world(struct game_state * gs,int x, int y)
{
	struct vector ret;
#define N_X_TILES 14.0
#define N_Y_TILES 18.0
#define TILE_WIDTH (640.0/N_X_TILES)
#define TILE_HEIGHT (480.0/N_Y_TILES)
	x -= (window_width/2);
	y -= (window_height/2);

	double virtual_Tile_X = x / TILE_WIDTH;
	double virtual_Tile_Y = y / TILE_HEIGHT;

	ret.x=virtual_Tile_X+virtual_Tile_Y;
	ret.y=0;
	ret.z=virtual_Tile_Y-virtual_Tile_X;

	ret.x += gs->game_player.location.x;
	ret.z += gs->game_player.location.z;

	return ret;
}

static inline struct vector pixel_to_screen(int x, int y)
{
	struct vector v;
	x -= (window_width/2);
	y -= (window_height/2);
	v.x=x;
	v.y=y;
	v.x/=window_width;
	v.y/=window_height;
	v.x*=(frame_x*2);
	v.y*=(frame_y*2);
	v.x=-v.x;
	v.y=-v.y;
	return v;
}
 
#endif
