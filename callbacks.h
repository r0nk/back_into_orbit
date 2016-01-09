#ifndef CALLBACKS
#define CALLBACKS

extern struct player_input pi;

void process_events();

void err_callback(int err, const char * description);
/* TODO
void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods);
void cursor_callback(GLFWwindow * win, double xpos, double ypos);
void cursor_button_callback(GLFWwindow * win, int button, int action, int mods);
*/
void simple_press_callback();
void play_callback();
void exit_callback(void);

#endif
