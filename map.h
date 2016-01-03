#ifndef MAP
#define MAP

#include "room.h"

#define MAX_ROOMS 10
#define MAX_EDGES 20

/* basically the portal struct */
struct edge {
	int a;
	int b;
};

static inline void dump_edge(struct edge * e)
{
	printf("%i -> %i\n",e->a,e->b);
}

struct map {
	struct room * current_room;
	int n_doorways;
	int n_rooms;
	struct room room[MAX_ROOMS];
	int n_edges;
	struct edge edge[MAX_EDGES];
	int level;/* current floor level */
};

static inline void dump_map(struct map * map)
{
	printf("\n --- dumping map --- \n");
	printf("map->n_rooms: %i\n",map->n_rooms);
	printf("map->current_room: %p\n",map->current_room);
	printf("map->n_edges: %i\n",map->n_edges);
	int i;
	for(i=0;i<map->n_rooms;i++){
		printf("map->room[%i]:\n",i);
		dump_room(&(map->room[i]));
	}
	for(i=0;i<map->n_edges;i++){
		printf("map->edge[%i]: ",i);
		dump_edge(&(map->edge[i]));
	}
}

struct map generate_map();
void generate_edges(struct map * map);
void transfer_map(int level);

void move_through_doorway(struct map * map,int t);

extern struct map world_map;
 
#endif
