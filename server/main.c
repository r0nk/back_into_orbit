#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <protolol.h>
#include "client.h"

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

	init_clients();

	return ss;
}

void accept_loop()
{
	int np = 0;
	while(1){
		np = accept(server_socket,NULL,NULL);
		add_client(np);
	}
}

void update_all()
{
		pthread_mutex_lock(&clients_mutex);
		/*TODO: make the magic happen in here*/
		sleep(1);
		pthread_mutex_unlock(&clients_mutex);
}

int main()
{
	server_socket = init_server();
	pthread_t accept_thread;
	pthread_create(&accept_thread,NULL,accept_loop,NULL);

	while(1)
		update_all();
}
