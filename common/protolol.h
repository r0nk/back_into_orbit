#ifndef PROTOLOL
#define PROTOLOL 1

/* protolol is the networking protocol both the server and the client share */

#include <game_state.h>
#include <string.h>
#include <unistd.h>

#define PROTOLOL_PORT 2690

struct protolol_packet {
	char magic_start[4];// should be "oHai"
	char type;// whether or not its a game state or kick or whatever
	char data[500];
};

#define PROTOLOL_TYPE_GAME_STATE 0

static inline void send_protolol(struct protolol_packet pp,int fd)
{
	write(fd,&pp,sizeof(pp));
}

static inline struct protolol_packet recv_protolol(int fd)
{
	struct protolol_packet pp;
	read(fd,&pp,sizeof(pp));
	return pp;
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

static inline struct game_state recv_game_state(int fd)
{
	struct protolol_packet pp;
	struct game_state gs;
	pp = recv_protolol(fd);
	if(pp.type!=PROTOLOL_TYPE_GAME_STATE){
		printf("non-recognized packet \n");
		return gs;//FIXME this shouldn't return, it should panic
	}
	memcpy(&gs,&pp.data,sizeof(gs));
	return gs;
}
 
#endif
