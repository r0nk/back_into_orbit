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

struct game_state world_state;

struct game_state init_game()
{
	int i;
	struct game_state gs;
	gs.n_players=1;
	gs.current_player=0;
	for(i=0;i<MAX_PLAYERS;i++){
		gs.game_player[i].speed = 0;
		gs.game_player[i].location = (struct vector) {0,0,0};
		gs.game_player[i].destination = (struct vector) {0,0,0};
	}
	gs.game_player[gs.current_player].location.x=5;
	gs.game_player[gs.current_player].location.y=0;
	gs.game_player[gs.current_player].location.z=5;
	gs.game_player[gs.current_player].speed=3.0;
	gs.game_player[gs.current_player].health=100;
	gs.n_bullets=0;
	return gs;
}

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

	e = listen(ss,10);
	if(e)
		err(3,"init_server() listen");

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

int get_client_inputs()
{
	int i;
	for (i=0;i<n_clients;i++){
		clients[i].pi = recv_player_input(clients[i].fd);
	}
	return 0;
}

void update_clients(struct game_state gs)
{
	int i;
	for(i=0;i<n_clients;i++)
	{
		gs.current_player=i;
		send_game_state(gs,clients[i].fd);
	}
}

void npc_update(struct game_state * gs)
{
	gs->n_npcs=1;
	gs->npc[0].location.x=5;
	gs->npc[0].location.y=0;
	gs->npc[0].location.z=5;

	if(clients[0].pi.keys['W'])
		gs->game_player[0].location.x=10;
	else
		gs->game_player[0].location.x=5;


	if(gs->n_players > 0)
		gs->npc[0].location = gs->game_player[0].location;

	if(gs->npc[0].location.x>10 || gs->npc[0].location.x<0)
		gs->npc[0].location.x=5;
	if(gs->npc[0].location.z>10 || gs->npc[0].location.z<0)
		gs->npc[0].location.z=5;

	if((gs->npc[0].location.x ==  gs->game_player[0].location.x) &&
	(gs->npc[0].location.z ==  gs->game_player[0].location.z)){
		gs->game_player[0].health-=0.100;
	}
}

void update_all()
{
	pthread_mutex_lock(&clients_mutex);
	get_client_inputs();
	npc_update(&world_state);
	update_clients(world_state);
	pthread_mutex_unlock(&clients_mutex);
}

int main()
{
	server_socket = init_server();
	pthread_t accept_thread;
	pthread_create(&accept_thread,NULL,accept_loop,NULL);
	world_state = init_game();

	printf("Waiting for clients to connect...\n");
	while(n_clients<1)
		sleep(1);//wait for clients to connect

	while(1)
		update_all();
}
