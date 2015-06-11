#include <GLFW/glfw3.h>
#include "callbacks.h"
#include "poly.h"

GLFWwindow * window;
int window_width,window_height;

int init_graphics(){
	glfwSetErrorCallback(err_callback);
	if(!glfwInit())
		return 0;
	window = glfwCreateWindow(640, 480, "lost_lair", NULL, NULL);
	if(!window)
		return 0;
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwGetFramebufferSize(window, &window_width, &window_height);
	glViewport(0,0,window_width,window_height);
}

void deinit_graphics(){
	glfwTerminate();
}

void draw_poly(struct polygon p){
	glColor3f(p.points[0].red,p.points[0].green,p.points[0].blue);
	glVertex2f(p.points[0].x,p.points[0].y);
	glColor3f(p.points[1].red,p.points[1].green,p.points[1].blue);
	glVertex2f(p.points[1].x,p.points[1].y);
	glColor3f(p.points[2].red,p.points[2].green,p.points[2].blue);
	glVertex2f(p.points[2].x,p.points[2].y);
}

void draw_polygons(){
	/* for each polygon,*/
		/* draw it*/
	glColor3f(1.f,0.f,0.f);
	glVertex2f(-0.6f,0.4f);
	glColor3f(0.f,1.f,0.f);
	glVertex2f(0.6f,-0.4f);
	glColor3f(0.f,0.f,1.f);
	glVertex2f(0.f,0.6f);
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio= window_width / (float) window_height;
	glOrtho(-ratio,ratio,-1.f,1.f,1.f,-1.f);
	glMatrixMode(GL_MODELVIEW);

	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	
	glBegin(GL_TRIANGLES);
	draw_polygons();
	glEnd();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
