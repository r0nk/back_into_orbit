#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <pthread.h>
#include <readline/readline.h>

#include <protolol.h>
#include <net_utils.h>

#include "servling.h"
#include "client.h"
#include "map.h"
#include "networking.h"
#include "cmd.h"

int servling_over_socket;
int client_over_socket;


void client_handler_loop()
{
	int np = 0;
	while(1){
		np = accept(client_over_socket,NULL,NULL);
		client_fd=np;
		add_player_to_servling(np,&servling[0]);
	}
}

void servling_acceptor()
{
	int np = 0;
	while(1){
		np = accept(servling_over_socket,NULL,NULL);
		add_servling(np);
	}
}

void add_player_to_servling(int pfd, struct servling * s)
{
	if(pfd==0 || s->fd==0){
		printf("ERR: added player to invalid output, returning\n");
		printf("	pfd:%i\n",pfd);
		printf("	s: %p\n",s);
		printf("	s->fd %i\n",s->fd);
		return;
	}
	servling_expect_client(s->fd);
	client_connect_to(pfd,s->ascii_ip);
	printf("adding client to sfd %i\n",s->fd);
}


int main()
{
	pthread_t servling_thread,client_thread;

	overmap = init_map();

	servling_over_socket=init_server(PROTOLOL_SOVER_PORT);
	client_over_socket=init_server(PROTOLOL_OVER_PORT);
	pthread_create(&servling_thread,NULL,servling_acceptor,NULL);
	pthread_create(&client_thread,NULL,client_handler_loop,NULL);

	start_servling();
	start_servling();

	while(do_cmd(readline("0> ")));

	close(servling_over_socket);
	close(client_over_socket);
}
