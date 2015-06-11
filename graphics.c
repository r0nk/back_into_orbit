#include <GLFW/glfw3.h>
#include "callbacks.h"
#include "game.h"
#include "poly.h"

GLFWwindow * window;
int window_width,window_height;

float ratio;

int init_graphics()
{
	glfwSetErrorCallback(err_callback);
	if(!glfwInit())
		return 0;
	window = glfwCreateWindow(640, 480, "corvus", NULL, NULL);
	if(!window)
		return 0;
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwGetFramebufferSize(window, &window_width, &window_height);
	glViewport(0,0,window_width,window_height);
	ratio = window_width / (float) window_height;
	glEnable(GL_DEPTH_TEST);
}

void deinit_graphics()
{
	glfwTerminate();
}

void draw_poly(struct polygon p)
{
	glColor3f(p.color[0].r,p.color[0].g,p.color[0].b);
	glVertex3f(p.point[0].x,p.point[0].y,p.point[0].z);
	glColor3f(p.color[1].r,p.color[1].g,p.color[1].b);
	glVertex3f(p.point[1].x,p.point[1].y,p.point[1].z);
	glColor3f(p.color[2].r,p.color[2].g,p.color[2].b);
	glVertex3f(p.point[2].x,p.point[2].y,p.point[2].z);
}

void draw_model(struct model model)
{
	int i;
	for(i=0;i<model.cardinality;i++)
		draw_poly(model.poly[i]);
}

void draw_models(){
	draw_model(p.model);
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-ratio,ratio,-1.f,1.f,1.f,-1.f);
	glMatrixMode(GL_MODELVIEW);
	
	glBegin(GL_TRIANGLES);
	draw_models();
	glEnd();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
