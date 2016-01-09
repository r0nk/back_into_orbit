#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "input.h"
#include "callbacks.h"
#include "engine.h"
#include "game.h"
#include "graphics.h"


void err_callback(int err, const char* description)
{
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(SDL_Event event)
{
	int key = event.key.keysym.sym;
	int action = event.key.state == SDL_PRESSED;
	printf("key pressed: %i\n",key);
	if(key>400 && key!=342)
		err(-23,"key>255, key=%i",key);
	if(key==256 && action ==0){
		if(!is_game_over)
			paused=!paused;
	}
	pi.keys[key]=(char)action; /*action is a binary (press||depress)*/
}

void cursor_callback(SDL_Event event)
{
	pi.mouse_x=event.motion.x;
	pi.mouse_y=event.motion.y;
}

void cursor_button_callback(SDL_Event event)
{
	int button = event.button.button;
	int action = (event.button.type == SDL_MOUSEBUTTONDOWN);
	if(button==SDL_BUTTON_LEFT)
		pi.left_click=action;
	if(button==SDL_BUTTON_RIGHT)
		pi.right_click=action;
}

void simple_press_callback()
{
	printf("simple press callback\n");
}

void play_callback()
{
	in_main_menu=0;
	paused=0;
}

void exit_callback()
{
	exit(0);
}

void process_events()
{
	/* Our SDL event placeholder. */
	SDL_Event event;

	/* Grab all the events off the queue. */
	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
			case SDL_KEYDOWN:
				key_callback(event );
				break;
			case SDL_QUIT:
				exit(0);
				break;
		}
	}
}
