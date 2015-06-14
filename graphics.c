#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "graphics.h"
#include "callbacks.h"
#include "game.h"
#include "poly.h"
#include "map.h"

GLFWwindow * window;
int window_width,window_height;
struct camera camera;

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


	camera.eye.x=1.0; camera.eye.y=4.0; camera.eye.z=6.0;
	camera.rot.x=0; camera.rot.y=0; camera.rot.z=0;
	camera.up.x=0; camera.up.y=1; camera.up.z=0;

	return 0;
}

void deinit_graphics()
{
	glfwTerminate();
}

void draw_vertice(struct vertice v)
{
	glNormal3f(v.n.x, v.n.y, v.n.z);
	glColor3f( v.c.x, v.c.y, v.c.z);
	glVertex3f(v.p.x, v.p.y, v.p.z);
}

void draw_poly(struct polygon p)
{
	draw_vertice(p.v[0]);
	draw_vertice(p.v[1]);
	draw_vertice(p.v[2]);
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

void draw_block(int x, int y, int z)
{
	unsigned int i;
	glPushMatrix();
	glTranslatef((double)x,(double)y,(double)z);
	glBegin(GL_TRIANGLES);
	for(i=0;i<block.cardinality;i++)
		draw_poly(block.poly[i]);
	glEnd();
	glPopMatrix();
}

void draw_map()
{
	int x,y,z;
	for(x=0;x<20;x++){
		for(y=0;y<20;y++){
			for(z=0;z<20;z++){
				if(world_map.tiles[x][y][z])
					draw_block(x,y,z);
			}
		}
	}
}

void draw_models(){
	draw_map();
	draw_model(p.model);
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-1.0,1.0,-1.0f,1.0f,1.0f,100.0f);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	gluLookAt(camera.eye.x,camera.eye.y,camera.eye.z,
			camera.eye.x+camera.rot.x,
			camera.eye.y+camera.rot.y,
			camera.eye.z+camera.rot.z,
			0,1,0);
	draw_models();

	glPopMatrix();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
