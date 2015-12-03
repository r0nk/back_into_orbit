#include <protolol.h>
#include "servling.h"
#include "networking.h"

void handle_servling_packet(int fd)
{
	struct protolol_packet pp;
	pp = recv_protolol(fd);
	switch(pp.type){
		case PROTOLOL_TYPE_CLIENT_TELEPORT:
			/*TODO fix this so it actually gives a client*/
			printf("overlord recieved client_teleport");
			client_connect_to(client_fd,"127.0.0.1");
			break;
		default:
			printf("overlord recieved servling packet type defaulted, pp.type:%i, fd:%i\n",pp.type,fd);
			err(-20,"overlord recived unknown packet type");
			break;
	}
}

void servling_handler(void * servling_fd)
{
	int * s = (int * ) servling_fd;
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
			servling_handler,&servling[n_servlings].fd);
	n_servlings++;
}

void start_servling()
{
	if(!fork())
		execl("../server/dr0ne_server","127.0.0.1");
}
