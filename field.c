#include "map.h"
#include "field.h"

static inline void propogate(struct vector * to, struct vector * from,
		double delta)
{
	if(!to||!from)
		return;
	double d = (2*(delta))*(from->y - to->y);
	to->x +=d;
	to->z +=d;
	from->x -=d;
	from->z -=d;
}

void update_cell(struct field * field,int i, int j,double delta)
{
	if(!walkable(i,j))
		return;
	struct vector *s = &(field->cell[i][j]);/*the center piece*/

	struct vector *a = (walkable(i-1,j)) ? &(field->cell[i-1][j]) :NULL;
	struct vector *w = (walkable(i,j+1)) ? &(field->cell[i][j+1]) :NULL;
	struct vector *d = (walkable(i+1,j)) ? &(field->cell[i+1][j]) :NULL;
	struct vector *x = (walkable(i,j-1)) ? &(field->cell[i][j-1]) :NULL;
	propogate(s,w,delta);
	propogate(s,a,delta);
	propogate(s,d,delta);
	propogate(s,x,delta);
}

void update_field(struct field * field, double delta)
{
	int i,j;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			update_cell(field,i,j,delta);
		}
	}
	double density = 100;
	double r = 1/density;
	for(i=0;i<MAX_ROOM_WIDTH;i++){
		for(j=0;j<MAX_ROOM_HEIGHT;j++){
			field->cell[i][j].y+=
				r*(field->cell[i][j].x + field->cell[i][j].z);
			field->cell[i][j].y/=1.005;
		}
	}
}

