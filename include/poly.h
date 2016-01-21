#ifndef POLY
#define POLY

#include <stdio.h>

struct vector {
	double x,y,z;
};

typedef struct vector point;

static inline void dump_vector(struct vector v)
{
	printf("(%f,%f,%f)",v.x,v.y,v.z);
}

#endif
