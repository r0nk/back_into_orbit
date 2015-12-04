#include <protolol.h>
#include "servling.h"
#include "networking.h"

void handle_servling_packet(struct servling s)
{
	struct protolol_packet pp;
	pp = recv_protolol(s.fd);
	switch(pp.type){
		case PROTOLOL_TYPE_CLIENT_TELEPORT:
			/*TODO fix this so it actually gives a client*/
			add_player_to_servling(client_fd,&servling[1]);
			break;
		default:
			printf("overlord recieved servling packet type defaulted, pp.type:%i, fd:%i\n",pp.type,s.fd);
			err(-20,"overlord recived unknown packet type");
			break;
	}
}

void servling_handler(void * servl)
{
	struct servling * s = (struct servling * ) servl;
	while(1)
		handle_servling_packet(*s);
}

void add_servling(int fd)
{
	if(n_servlings>=MAX_SERVLINGS)
		return;
	servling[n_servlings].fd=fd;
	servling[n_servlings].ascii_ip="127.0.0.1";
	pthread_create(&servling[n_servlings].thread,NULL,
			servling_handler,(void *)&servling[n_servlings]);
	n_servlings++;
}

void start_servling()
{
	if(!fork())
		execl("../server/dr0ne_server","127.0.0.1");
}
