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

struct game_state get_state_from_clients(){
	struct game_state ret_gs,buffer_gs;
	int i;

	ret_gs.n_players=n_clients;
	for(i=0;i<n_clients;i++)
	{
		buffer_gs = recv_game_state(clients[i].fd);
		ret_gs.game_player[i]=buffer_gs.game_player[i];
	}
	return ret_gs;
}

void update_clients(struct game_state gs){
	int i;
	for(i=0;i<n_clients;i++)
	{
		gs.current_player=i;
		send_game_state(gs,clients[i].fd);
	}
}

void update_all()
{
	while(n_clients<1)
		sleep(1);//wait for clients to connect

	pthread_mutex_lock(&clients_mutex);
	struct game_state gs;
	gs = get_state_from_clients();
	update_clients(gs);
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
