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

#include "servling.h"
#include "client.h"

int servling_over_socket;
int client_over_socket;

void client_handler_loop()
{
	int np = 0;
	while(1){
		np = accept(client_over_socket,NULL,NULL);
		printf("Overlord accepted client connection: %i\n",np);
		client_connect_to(np,"127.0.0.1");
	}
}

void servling_handler_loop()
{
	int np = 0;
	while(1){
		np = accept(servling_over_socket,NULL,NULL);
		printf("Overlord accepted server connection: %i\n",np);
		add_servling(np);
	}
}

void start_servling()
{
	if(!fork())
		execl("../server/dr0ne_server","127.0.0.1");
}

void add_player_to_servling(struct client * p, struct servling * s)
{
	// tell servling its about to get a player p
	client_connect_to(p->fd,s->ascii_ip);
}

int init_server(int port)
{
	struct sockaddr_in address;
	int e = 0;
	int ss = socket(PF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr ("127.0.0.1");
	address.sin_port = htons(port);

	e = bind(ss,(struct sockaddr *)&address,sizeof(struct sockaddr_in));
	if(e)
		err(2,"init_server() binding");

	e = listen(ss,10);
	if(e)
		err(3,"init_server() listen");
	return ss;
}

int main()
{
	pthread_t servling_thread,client_thread;
	servling_over_socket=init_server(PROTOLOL_SOVER_PORT);
	client_over_socket=init_server(PROTOLOL_OVER_PORT);
	pthread_create(&servling_thread,NULL,servling_handler_loop,NULL);
	pthread_create(&client_thread,NULL,client_handler_loop,NULL);

	start_servling();
	start_servling();

	while(strcmp(readline("o> "),"exit"));

	close(servling_over_socket);
	close(client_over_socket);
}
