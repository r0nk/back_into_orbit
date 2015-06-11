#ifndef POLY
#define POLY

struct color{
	float r,g,b;
};

struct point{
	float x,y,z;
};

struct polygon{
	struct point point[3];
	struct color color[3];
};

struct model{
	unsigned int cardinality; 
	struct polygon *poly;
};

#endif
