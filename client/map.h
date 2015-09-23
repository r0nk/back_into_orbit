#ifndef MAP
#define MAP

struct map {
	char tiles[100][100];
};

extern struct map world_map;
extern struct model block;

struct map mkmap(char * pathname);

#endif
