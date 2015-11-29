#ifndef PROTOLOL
#define PROTOLOL 1

/* protolol is the networking protocol both the server and the client share */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <game_state.h>
#include <input.h>
#include <room.h>

#define PROTOLOL_PORT 2690

struct protolol_packet {
	char magic_start[4];// should be "oHai"
	char type;// whether or not its a game state or kick or whatever
	char data[sizeof(struct game_state)];
};

/* client-server */
#define PROTOLOL_TYPE_GAME_STATE 1
#define PROTOLOL_TYPE_PLAYER_INPUT 2
#define PROTOLOL_TYPE_ROOM_CHANGE 3
/* overlord-server */
#define PROTOLOL_TYPE_EXPECT_CLIENT 4
#define PROTOLOL_TYPE_DROP_CLIENT 5
#define PROTOLOL_TYPE_CLIENT_TELEPORT 6
/* overlord-client */
#define PROTOLOL_TYPE_CONNECT_TO 7

static inline void send_protolol(struct protolol_packet pp,int fd)
{
	if(pp.type==PROTOLOL_TYPE_GAME_STATE)
		write(fd,&pp,sizeof(pp));
	else if (pp.type==PROTOLOL_TYPE_PLAYER_INPUT)
		write(fd,&pp,5+sizeof(struct player_input));
	else if (pp.type==PROTOLOL_TYPE_ROOM_CHANGE)
		write(fd,&pp,5+sizeof(struct room));
}

static inline struct protolol_packet recv_protolol(int fd)
{
	struct protolol_packet pp;
	read(fd,&pp,sizeof(pp));
	return pp;
}

static inline void send_player_input(struct player_input pi ,int fd)
{
	struct protolol_packet pp;
	pp.magic_start[0]='o';
	pp.magic_start[1]='H';
	pp.magic_start[2]='a';
	pp.magic_start[3]='i';
	pp.type = PROTOLOL_TYPE_PLAYER_INPUT;
	memcpy(&pp.data,&pi,sizeof(pi));
	send_protolol(pp,fd);
}

static inline void send_game_state(struct game_state gs ,int fd)
{
	struct protolol_packet pp;
	pp.magic_start[0]='o';
	pp.magic_start[1]='H';
	pp.magic_start[2]='a';
	pp.magic_start[3]='i';
	pp.type = PROTOLOL_TYPE_GAME_STATE;
	memcpy(&pp.data,&gs,sizeof(gs));
	send_protolol(pp,fd);
}

static inline void send_room(struct room r,int fd)
{
	struct protolol_packet pp;
	pp.magic_start[0]='o';
	pp.magic_start[1]='H';
	pp.magic_start[2]='a';
	pp.magic_start[3]='i';
	pp.type = PROTOLOL_TYPE_ROOM_CHANGE;
	memcpy(&pp.data,&r,sizeof(r));
	send_protolol(pp,fd);
}


static inline struct player_input recv_player_input(int fd)
{
	struct protolol_packet pp;
	struct player_input pi;
	pp = recv_protolol(fd);
	if(pp.type!=PROTOLOL_TYPE_PLAYER_INPUT){
		printf("non-recognized packet; not player input \n");
		exit(4);
	}
	memcpy(&pi,&pp.data,sizeof(pi));
	return pi;
}

static inline struct game_state recv_game_state(int fd)
{
	struct protolol_packet pp;
	struct game_state gs;
	pp = recv_protolol(fd);
	if(pp.type!=PROTOLOL_TYPE_GAME_STATE){
		printf("non-recognized packet; not game state \n");
		exit(3);
	}
	memcpy(&gs,&pp.data,sizeof(gs));
	return gs;
}

#endif
