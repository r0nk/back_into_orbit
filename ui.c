#include "ui.h"
#include "input.h"
#include "engine.h"

void update_button(struct button * b,struct vector pmv)
{
	if((pmv.x>b->location.x) && (pmv.x< (b->location.x+b->size.x)) &&
		(pmv.y>b->location.y) && (pmv.y< (b->location.y+b->size.y))){
		b->color.x=0.5;

		if(pi.left_click){
			if(!b->down){
				b->down=1;
			}
		}else{
			if(b->down && b->callback)
				b->callback();
			b->down=0;
		}
	}else{
		b->color.x=0.0;
		b->down=0;
	}
}

void update_ui(struct ui * ui)
{
	int i;
	struct vector pmv = pixel_to_screen(pi.mouse_x,pi.mouse_y);
	for(i=0;i<ui->n_buttons;i++){
		update_button(&(ui->button[i]),pmv);
	}
}

void init_ui()
{
	paused_ui = paused_menu();
	main_menu_ui = main_menu();
	options_ui = options_menu();
	gameover_ui = gameover_menu(0,0);
	ui = &paused_ui;
	in_options=0;
	in_main_menu=1;
}
