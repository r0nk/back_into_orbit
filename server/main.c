#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <protolol.h>

int server_socket;

int init_server()
{
	struct sockaddr_in address;
	int e = 0;
	int ss = socket(PF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_port = htons(PROTOLOL_PORT);

	e = bind(ss,(struct sockaddr *)&address,sizeof(struct sockaddr_in));
	if(e)
		err(2,"init_server() binding");
	printf("socket binded.\n");

	e = listen(ss,10);
	if(e)
		err(3,"init_server() listen");
	printf("socket listening.\n");

	return ss;
}

/* start a new thread for each new player */
void handle(int player_fd)
{
	printf("TODO handle %i \n",player_fd);
	write(player_fd,"oHai\n",5);
	sleep(1);
}

void accept_loop()
{
	int np = 0;
	while(1){
		np = accept(server_socket,NULL,NULL);
		handle(np);
	}
}

int main()
{
	server_socket = init_server();
	pthread_t accept_thread;
	pthread_create(&accept_thread,NULL,accept_loop,NULL);

	while(1){
		printf(".\n");
		sleep(10);
	}
	/* TODO main universe loop */
}
