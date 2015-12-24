#include "ui.h"
#include "input.h"

void update_button(struct button * b,struct vector pmv)
{
	if((pmv.x>b->location.x) && (pmv.x< (b->location.x+b->size.x)) &&
		(pmv.y>b->location.y) && (pmv.y< (b->location.y+b->size.y))){

		b->color.y=1;
	}else{
		b->color.y=0;
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
