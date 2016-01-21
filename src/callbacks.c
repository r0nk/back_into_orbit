#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "input.h"
#include "callbacks.h"
#include "engine.h"
#include "game.h"
#include "graphics.h"
#include "audio.h"


void err_callback(int err, const char* description)
{
	printf("ERR CALLBACK[%i]:%s\n",err,description);
}

void key_callback(SDL_Event event)
{
	int key = event.key.keysym.sym;
	int action = event.key.state == SDL_PRESSED;
	if(key>400 && key!=342){
		printf("key>400: %c\n",key);
		return;
	}

	if(key==27 && action ==0){
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

void options_callback()
{
	in_options=1;
}

void options_back_callback()
{
	in_options=0;
}

void toggle_audio_callback()
{
	audio_enabled=!audio_enabled;
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
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				cursor_button_callback(event);
				break;
			case SDL_MOUSEMOTION:
				cursor_callback(event);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				key_callback(event );
				break;
			case SDL_QUIT:
				exit(0);
				break;
		}
	}
}
