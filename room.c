#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "layouts.h"
#include "room.h"

void get_layout(struct room * room,char * pathname)
{
	int fd = open(pathname,O_RDONLY);
	if(fd==-1)
		err(-29,"get_layout() couldn't open");

	char c[2];
	int x=0,y=0;

	memset(&room->layout,0,sizeof(struct layout));

	while(read(fd,c,1)){
		switch(c[0]){
		case '\n':
			y++;
			x=0;
			continue;
		case ' ':
			room->layout.tiles[x][y]='\0';
			break;
		case 's':
			room->layout.tiles[x][y]='s';
			break;
		default:
			room->layout.tiles[x][y]=c[0];
			break;
		}
		x++;
	}
}

void count_doorways(struct room * room)
{
	int n = 0;
	int x,z;
	for(z=0;z<100;z++){
		for(x=0;x<100;x++){
			if((room->layout.tiles[x][z]) > ('0'-1)
					&& room->layout.tiles[x][z]<'9'){
				room->doorway[n].x=x;
				room->doorway[n].z=z;
				room->doorway[n].is_connected=0;
				n++;
			}
		}
	}
	room->n_doorways=n;
}

struct vector pick_colors(int i)
{
	switch(i){
		case 0:
			return (struct vector) {0,0,230};
			break;
		case 1:
			return (struct vector) {0,230,0};
			break;
		case 2:
			return (struct vector) {0,230,230};
			break;
		case 3:
			return (struct vector) {230,0,0};
			break;
		case 4:
			return (struct vector) {230,0,230};
			break;
		case 5:
			return (struct vector) {230,230,0};
			break;
		case 6:
			return (struct vector) {230,230,230};
			break;
		default:
			return (struct vector) {rand()%255,rand()%255,rand()%255};
			break;
	}
}

struct model model_room(struct room * room)
{
	struct model m = wall_block(room->color,(struct vector) {1,2,1},(struct vector){0,0,0});
	struct model w = wall_block(room->color,(struct vector) {1,2,1},(struct vector){0,0,0});
	struct model f = floor_tile((struct vector) {1,0,1},(struct vector){0,0,0});
	int x,z;
	for(x=0;x<100;x++){
		for(z=0;z<100;z++){
			if(room->layout.tiles[x][z]==LAYOUT_WALL){
				free(w.poly);
				w = wall_block(room->color,(struct vector) {1,2,1},(struct vector){x,0,z});
				add_submodel(&m,&w);
			}
			if(room->layout.tiles[x][z]){
				free(f.poly);
				f = floor_tile((struct vector) {1,0,1},(struct vector){x,0,z});
				add_submodel(&m,&f);
			}
		}
	}
	printf("modeled room, cardinality: %i\n",m.cardinality);
	return m;
}

struct room generate_room(int i)
{
	struct room room;
	pick_layout(&room);
	count_doorways(&room);
	room.gs = init_game(&room);
	room.color= pick_colors(i);
	room.model = model_room(&room);
	return room;
}
