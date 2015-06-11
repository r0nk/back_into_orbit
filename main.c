#include <stdio.h>
#include "graphics.h"

int main(){
	if(!init_graphics())
		return 1;
	if(!init_game())
		return 2;
	while(1)
		draw();
	return 0;
}

