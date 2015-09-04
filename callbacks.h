#ifndef CALLBACKS
#define CALLBACKS
#include <GLFW/glfw3.h>

int cursor_x;
int cursor_y;

void err_callback(int err, const char * description);
void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods);
void cursor_callback(GLFWwindow * win, double xpos, double ypos);
void cursor_button_callback(GLFWwindow * win, int button, int action, int mods);

#endif
