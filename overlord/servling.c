#include "servling.h"

void servling_handler(void * servling)
{
	struct servling s = (struct servling) servling;
	while(1)
		handle_servling_packet(servling.fd);
}

void add_servling(int fd)
{
	if(n_servlings>=MAX_SERVLINGS)
		return;
	n_servlings++;
	servling[n_servlings].fd=fd;
	servling[n_servlings].ascii_ip="127.0.0.1";
	pthread_create(&servling[n_servlings].thread,NULL,
			servling_handler,&servling[n_servlings]);
}

void start_servling()
{
	if(!fork())
		execl("../server/dr0ne_server","127.0.0.1");
}