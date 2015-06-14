#ifndef MAP
#define MAP

struct map {
	char tiles[20][20][20];
};

extern struct map world_map;
extern struct model block;

struct map mkmap();

#endif
