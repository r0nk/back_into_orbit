#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "layouts.h"
#include "room.h"
#include "shop.h"

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
	room->has_shop=0;
	for(z=0;z<MAX_ROOM_HEIGHT;z++){
		for(x=0;x<MAX_ROOM_WIDTH;x++){
			if((room->layout.tiles[x][z]) > ('0'-1)
					&& room->layout.tiles[x][z]<'9'){
				room->doorway[n].location =(struct vector){x,0,z};
				room->doorway[n].is_connected=0;
				n++;
			}
			if(room->layout.tiles[x][z]=='K')
				room->has_shop=1;
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
			return (struct vector) {204,203,127};
			break;
		case 6:
			return (struct vector) {230,230,230};
			break;
		case 7:
			return (struct vector) {207,107,76};
			break;
		case 8:
			return (struct vector) {255,215,0};
			break;
		case 9:
			return (struct vector) {16,16,16};
			break;
		default:
			return (struct vector) {rand()%255,rand()%255,rand()%255};
			break;
	}
}

struct model model_floors(struct room * room)
{
	int x,z;
	struct vector d;
	struct model m = wall_block(room->color,(struct vector) {1,2,1},
			(struct vector){0,0,0});
	struct model f = floor_tile((struct vector) {1,2,1},
			(struct vector){0,0,0});
	char wm[MAX_ROOM_WIDTH][MAX_ROOM_HEIGHT];//boolean for done floors
	for(x=0;x<MAX_ROOM_WIDTH;x++){
		for(z=0;z<MAX_ROOM_HEIGHT;z++){
			if(room->layout.tiles[x][z] && !wm[x][z]){
				for(d.x=0;d.x<MAX_ROOM_WIDTH;d.x++){
					if(room->layout.tiles[(int)d.x+x][z] && !wm[(int)d.x+x][z]){
						wm[(int)d.x+x][z]=1;
					}else{
						break;
					}
				}

				free(f.poly);
				f = floor_tile((struct vector) {d.x,2,1},
						(struct vector){x,0,z});
				add_submodel(&m,&f);
				d = (struct vector) {0,0,0};
			}
		}
	}
	return m;
}

struct model model_walls(struct room * room)
{
	int x,z;
	struct vector d = (struct vector) {0,0,0};
	struct model m = wall_block(room->color,(struct vector) {1,2,1},
			(struct vector){0,0,0});
	struct model w = wall_block(room->color,(struct vector) {1,2,1},
			(struct vector){0,0,0});
	char wm[MAX_ROOM_WIDTH][MAX_ROOM_HEIGHT];//boolean for done walls
	
	for(x=0;x<MAX_ROOM_WIDTH;x++){
		for(z=0;z<MAX_ROOM_HEIGHT;z++){
			wm[x][z]=0;
		}
	}
	for(x=0;x<MAX_ROOM_WIDTH;x++){
		for(z=0;z<MAX_ROOM_HEIGHT;z++){
			if(room->layout.tiles[x][z]==LAYOUT_WALL && !wm[x][z]){
				for(d.x=0;d.x<MAX_ROOM_WIDTH;d.x++){
					if(room->layout.tiles[(int)d.x+x][z]==LAYOUT_WALL && !wm[(int)d.x+x][z]){
						wm[(int)d.x+x][z]=1;
					}else{
						break;
					}
				}

				free(w.poly);
				w = wall_block(room->color,
						(struct vector) {d.x,2,1},
						(struct vector){x,0,z});
				add_submodel(&m,&w);
				d = (struct vector) {0,0,0};
			}
		}
	}
	return m;
}

struct model model_room(struct room * room)
{
	struct model m = model_walls(room);
	struct model f = model_floors(room);
	add_submodel(&m,&f);
//	printf("modeled room, cardinality: %i\n",m.cardinality);
	return m;
}

struct room generate_room(int i,int level)
{
	struct room room;
	switch(i)
	{
		case 0:
			get_layout(&room,"layouts/starting.layout");
			break;
		case 8:
			get_layout(&room,"layouts/shop.layout");
			room.shop=generate_shop(&room);
			break;
		case 9:
			room.boss_room=level;
			if(level==1){
				get_layout(&room,"layouts/boss.layout");
			} else if(level==2){
				get_layout(&room,"layouts/mole.layout");
			} else if(level==3){
				get_layout(&room,"layouts/yoyo.layout");
			}
			break;
		default:
			pick_layout(&room);
			break;

	}
	count_doorways(&room);
	room.gs = init_game(&room);
	room.color= pick_colors(i);
	room.model = model_room(&room);
	return room;
}
