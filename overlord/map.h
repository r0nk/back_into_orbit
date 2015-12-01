#ifndef MAP
#define MAP

struct edge
{
	int a_index;/*servling array index*/
	int b_index;
	int a_door;
	int b_door;
}

#define MAX_EDGES 400

struct map
{
	struct edge edge[MAX_EDGES];
}
	
#endif
