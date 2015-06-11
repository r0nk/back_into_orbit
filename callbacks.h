#ifndef CALLBACKS
#define CALLBACKS
#include <GLFW/glfw3.h>

void err_callback(int err, const char * description);
void key_callback(GLFWwindow * win, int key, int scanc, int action, int mods);

#endif
