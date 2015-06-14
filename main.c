#include <unistd.h>
#include <stdio.h>
#include "graphics.h"
#include "game.h"

int main(){
	init_graphics();
	init_game();
	while(1){
		draw();
	}
	return 0;
}

