#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <pthread.h>

#include "networking.h"
#include <net_utils.h>

int overlord_fd;
int server_fd;

int connect_to_server(char * ip_addr)
{

	struct sockaddr_in address;
	int cs = socket(PF_INET,SOCK_STREAM,0);
	int e = 0;

	address.sin_family = AF_INET;
	address.sin_port = htons(PROTOLOL_PORT);
	inet_aton(ip_addr,&address.sin_addr);

	e = connect(cs,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(e)
		err(1,"init_networking()");

	return cs;
}


void handle_overlord_packet(int ofd)
{
	struct protolol_packet pp;
	pp = recv_protolol(ofd);
	switch(pp.type){
		case PROTOLOL_TYPE_CONNECT_TO:
			server_fd = connect_to_server(&pp.data);
			printf("connected to server %i\n",server_fd);
			break;
		default:
			printf("overlord to client packet defaulted, type:%i\n",
					pp.type);
			err(-8,"unrecognized protolol overlord packet type");
			break;
	}
}

void handle_server_packet(int sfd,struct game_state * gs, struct room * wr)
{
	struct protolol_packet pp;
	pp = recv_protolol(sfd);
	switch(pp.type){
		case PROTOLOL_TYPE_GAME_STATE:
			memcpy(gs,&pp.data,sizeof(*gs));
			break;
		case PROTOLOL_TYPE_ROOM_CHANGE:
			memcpy(wr,&pp.data,sizeof(*wr));
			break;
		default:
			err(-8,"unrecognized protolol packet type");
			break;
	}
}

struct game_state update_state(int server_fd, struct game_state gs,
		struct player_input pi)
{
	send_player_input(pi,server_fd);
	handle_server_packet(server_fd,&gs,&world_room);
	return gs;
}

void overlord_handler()
{
	while(1)
		handle_overlord_packet(overlord_fd);
}

int init_networking()
{
	pthread_t overlord_thread;
	overlord_fd = connect_to_overlord(PROTOLOL_OVER_PORT);
	pthread_create(&overlord_thread,NULL,overlord_handler,NULL);
	while(!server_fd); /* wait till we get a game server */
	return server_fd;
}
