#ifndef UI
#define UI 1

#include "poly.h"
#include "callbacks.h"

struct button
{
	struct vector location;
	struct vector size;
	char * text;
	struct vector color;
	struct vector text_color;
	void (*callback)(void);
	int down;
};

inline static void dump_button(struct button b){
	printf("button location: ");
	dump_vector(b.location);
	printf("\n");
	printf("button size: ");
	dump_vector(b.size);
	printf("\n");
	if(b.text!=NULL)
		printf("text: %s\n",b.text);
	printf("button text_color: ");
	dump_vector(b.text_color);
	printf("\n");
	printf("button color: ");
	dump_vector(b.color);
	printf("\n");
}

#define MAX_BUTTONS 10

struct ui
{
	int n_buttons;
	struct button button[MAX_BUTTONS];
};

inline static struct ui paused_menu()
{
	struct ui ui;
	ui.n_buttons=1;
	ui.button[0].location.x=-1;
	ui.button[0].location.y=-2;
	ui.button[0].size.x=2;
	ui.button[0].size.y=1;
	ui.button[0].down=0;
	ui.button[0].text="Exit";
	ui.button[0].color = (struct vector) {1,0.5,1};
	ui.button[0].callback = exit_callback;
	return ui;
}

inline static struct ui test_ui()
{
	struct ui ui;
	ui.n_buttons=1;
	ui.button[0].location.x=0;
	ui.button[0].location.y=6;
	ui.button[0].size.x=2;
	ui.button[0].size.y=1;
	ui.button[0].down=0;
	ui.button[0].text="test";
	ui.button[0].color = (struct vector) {1,0.5,1};
	ui.button[0].callback = simple_press_callback;

	return ui;
}

struct ui * ui;
struct ui paused_ui;
 
#endif
