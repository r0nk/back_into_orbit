#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

#include <room.h>
#include <protolol.h>

#include "client.h"
#include "game_logic.h"

int overlord_fd;

int server_socket;

struct game_state world_state;
struct room world_room;

struct game_state init_game()
{
	int i;
	struct game_state gs;
	gs.n_players=1;
	gs.current_player=0;
	for(i=0;i<MAX_PLAYERS;i++){
		gs.game_player[i].speed = 2.0;
		gs.game_player[i].health = 100;
		gs.game_player[i].location = (struct vector) {2+i,0,2};
		gs.game_player[i].destination = (struct vector) {0,0,0};
		gs.game_player[i].team = BLUE_TEAM;
		gs.game_player[i].type = UNIT_TYPE_PLAYER;
	}
	gs.n_bullets=0;
	gs.red.flag_starting = (struct vector){ 45,0,15};
	gs.blue.flag_starting = (struct vector){ 5,0,5};
	gs.red.spawn = (struct vector){ 40,0,15};
	gs.blue.spawn = (struct vector){ 5,0,8};
	gs.red.flag.location = gs.red.flag_starting;
	gs.blue.flag.location = gs.blue.flag_starting;
	gs.red.score=0;
	gs.blue.score=0;
	
	gs.n_npcs=3;
	gs.npc[0].speed = 0;
	gs.npc[0].health = 100;
	gs.npc[0].location = (struct vector) {28,0,2};
	gs.npc[0].destination = (struct vector) {0,0,0};
	gs.npc[0].team = 0;
	gs.npc[0].type = UNIT_TYPE_SHOP;

	gs.npc[1].speed = 0;
	gs.npc[1].health = 10;
	gs.npc[1].location = (struct vector) {20,0,2};
	gs.npc[1].destination = (struct vector) {0,0,0};
	gs.npc[1].team = 0;
	gs.npc[1].type = UNIT_TYPE_COIN;

	gs.npc[2].speed = 8.0;
	gs.npc[2].health = 100;
	gs.npc[2].location = (struct vector) {15,0,3};
	gs.npc[2].destination = (struct vector) {15,0,3};
	gs.npc[2].team = 0;
	gs.npc[2].type = UNIT_TYPE_NEUTRAL_CREEP;

	glfwInit();
	world_room=mkroom("../rooms/gamma.room");
	return gs;
}

int init_server()
{
	struct sockaddr_in address;
	int e = 0;
	int ss = socket(PF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr ("127.0.0.1");
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


void engine_tick(struct game_state * gs,double delta)
{
	player_update(gs,delta); //updates the players
	bullet_update(gs,delta);
	npc_update(gs,delta);
	flag_update(gs,delta);
}

double delta_time()
{
	double s = glfwGetTime();
	glfwSetTime(0.0);
	return s;
}

void update_all(struct game_state * gs)
{
	pthread_mutex_lock(&clients_mutex);

	gs->n_players=n_clients;
	get_client_inputs();
	engine_tick(gs,delta_time());
	update_clients(*gs);

	pthread_mutex_unlock(&clients_mutex);
}

int connect_to_overlord()
{
	struct sockaddr_in address;
	int cs = socket(PF_INET,SOCK_STREAM,0);
	int e = 0;

	address.sin_family = AF_INET;
	address.sin_port = htons(PROTOLOL_OVER_PORT);
	inet_aton("127.0.0.1",&address.sin_addr);

	e = connect(cs,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(e)
		err(1,"connect_to_overlord()");
	printf(" server Connected to overlord\n");

	return cs;
}

int main()
{
	overlord_fd = connect_to_overlord();
	server_socket = init_server();
	pthread_t accept_thread;
	pthread_create(&accept_thread,NULL,accept_loop,NULL);
	world_state = init_game();

	printf("Drone server: Waiting for clients to connect...\n");

	while(n_clients<1)
		sleep(1);//wait for clients to connect

	while(1)
		update_all(&world_state);
}
