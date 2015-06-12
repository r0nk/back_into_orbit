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
		return -1;
	window = glfwCreateWindow(640, 480, "corvus", NULL, NULL);
	if(!window)
		return -2;
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, cursor_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &window_width, &window_height);
	glViewport(0,0,window_width,window_height);
	ratio = window_width / (float) window_height;
	glClearColor(0.2,0.3,0.3,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glScalef(0.4f,0.4f,0.4f);

	return 0;
}

void deinit_graphics()
{
	glfwTerminate();
}

/* calculate and apply normal */
void norm(struct polygon p)
{
	struct point c,u,v;
	u = p.point[1];
	v = p.point[2];
	c.x = (u.y*v.z)-(u.z*v.y);
	c.y = (u.z*v.x)-(u.x*v.z);
	c.z = (u.x*v.y)-(u.y*v.x);
	glNormal3f(c.x,c.y,c.z);
}

void draw_poly(struct polygon p)
{
	norm(p);
	glColor3f(p.color[0].r,p.color[0].g,p.color[0].b);
	glVertex3f(p.point[0].x,p.point[0].y,p.point[0].z);
	glColor3f(p.color[1].r,p.color[1].g,p.color[1].b);
	glVertex3f(p.point[1].x,p.point[1].y,p.point[1].z);
	glColor3f(p.color[2].r,p.color[2].g,p.color[2].b);
	glVertex3f(p.point[2].x,p.point[2].y,p.point[2].z);
}

void draw_model(struct model model)
{
	unsigned int i;
	glPushMatrix();
	glTranslatef(p.location.x,p.location.y,p.location.z);
	glBegin(GL_TRIANGLES);
	for(i=0;i<model.cardinality;i++)
		draw_poly(model.poly[i]);
	glEnd();
	glPopMatrix();
}

void draw_models(){
	draw_model(p.model);
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-ratio,ratio,-1.f,1.f,1.f,-1.f);
	glMatrixMode(GL_MODELVIEW);

	glRotatef(0.1f,1.0f,1.0f,0.0f);
	draw_models();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
