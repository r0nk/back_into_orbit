#ifndef POLY
#define POLY

struct point{
	float x,y,red,green,blue;
};

struct polygon{
	struct point points[3];
};

#endif
