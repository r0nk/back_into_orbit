#ifndef UI
#define UI 1

#include "poly.h"
#include "callbacks.h"

struct text_field
{
	struct vector location;
	char * text;
	struct vector color;
};

inline static void dump_text_field(struct text_field t){
	printf("text_field location: ");
	dump_vector(t.location);
	printf("\n");
	if(t.text!=NULL)
		printf("text: %s\n",t.text);
	printf("color: ");
	dump_vector(t.color);
	printf("\n");
}

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
#define MAX_TEXT 5

struct ui
{
	int n_buttons;
	struct button button[MAX_BUTTONS];
	int n_text_fields;
	struct text_field text_field[MAX_TEXT];
};

inline static struct ui main_menu()
{
	struct vector color = (struct vector) {1,1,1};
	struct ui ui;
	ui.n_buttons=3;
	ui.n_text_fields=0;
	ui.button[0].location.x=-1;
	ui.button[0].location.y=-2.5;
	ui.button[0].size.x=3.5;
	ui.button[0].size.y=1;
	ui.button[0].down=0;
	ui.button[0].text="Exit";
	ui.button[0].color = color;
	ui.button[0].callback = exit_callback;

	ui.button[1].location.x=-1;
	ui.button[1].location.y=-1;
	ui.button[1].size.x=3.5;
	ui.button[1].size.y=1;
	ui.button[1].down=0;
	ui.button[1].text="Options";
	ui.button[1].color = color;
	ui.button[1].callback = exit_callback;

	ui.button[2].location.x=-1;
	ui.button[2].location.y=0.5;
	ui.button[2].size.x=3.5;
	ui.button[2].size.y=1;
	ui.button[2].down=0;
	ui.button[2].text="Play";
	ui.button[2].color = color;
	ui.button[2].callback = play_callback;
	return ui;
}

inline static struct ui paused_menu()
{
	struct ui ui;
	ui.n_buttons=1;
	ui.n_text_fields=1;

	ui.text_field[0].location.x=0;
	ui.text_field[0].location.y=0;
	ui.text_field[0].text="paused";
	ui.text_field[0].color = (struct vector) {0.0,1.0,1};

	ui.button[0].location.x=-1;
	ui.button[0].location.y=-2;
	ui.button[0].size.x=2;
	ui.button[0].size.y=1;
	ui.button[0].down=0;
	ui.button[0].text="Exit";
	ui.button[0].color = (struct vector) {0.5,0.5,1};
	ui.button[0].callback = exit_callback;
	return ui;
}

inline static struct ui test_ui()
{
	struct ui ui;
	ui.n_buttons=1;
	ui.n_text_fields=0;
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

inline static struct ui gameover_menu(int score,int win)
{
	struct vector color = (struct vector) {1,1,1};
	struct ui ui;
	ui.n_buttons=1;
	ui.n_text_fields=2;

	ui.text_field[0].location.x = 3;
	ui.text_field[0].location.y = 1;
	if(win){
		ui.text_field[0].text="YOU WIN";
		ui.text_field[0].color= (struct vector) {1,1,1};
	}else{
		ui.text_field[0].text="GAME OVER";
		ui.text_field[0].color= (struct vector) {1,0,0};
	}

	ui.text_field[1].location.x = 3;
	ui.text_field[1].location.y = 0;
	ui.text_field[1].text=calloc(50,1);
	sprintf(ui.text_field[1].text,"score: %i",score);
	ui.text_field[1].color= (struct vector) {0,1,1};

	ui.button[0].location.x=0;
	ui.button[0].location.y=-2.0;
	ui.button[0].size.x=3.5;
	ui.button[0].size.y=1;
	ui.button[0].down=0;
	ui.button[0].text="Exit";
	ui.button[0].color = color;
	ui.button[0].callback = exit_callback;

	return ui;
}

struct ui * ui;
struct ui paused_ui;
struct ui gameover_ui;
struct ui main_menu_ui;

void update_ui(struct ui * ui);
void init_ui();
 
#endif
