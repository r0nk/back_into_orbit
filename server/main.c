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
#include <net_utils.h>

#include "client.h"
#include "networking.h"
#include "game_logic.h"

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
	world_room=mkroom("../rooms/simple.room");
	return gs;
}

void handle_overlord_packet(int fd)
{
	struct protolol_packet pp;
	pp = recv_protolol(fd);
	switch(pp.type){
		case PROTOLOL_TYPE_EXPECT_CLIENT:
			should_accept=1;
			break;
		default:
			printf("server packet from overlord type defaulted, pp.type:%i, fd:%i\n",pp.type,fd);
			err(-20,"overlord recived unknown packet type");
			break;
	}
}

void overlord_handler()
{
	int np = 0;
	should_accept=0;
	while(1){
		handle_overlord_packet(overlord_fd);
		if(should_accept){
			np = accept(server_socket,NULL,NULL);
			add_client(np);
			should_accept=0;
		}
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


int main()
{
	overlord_fd = connect_to_overlord(PROTOLOL_SOVER_PORT);
	server_socket = init_server(PROTOLOL_PORT);
	init_clients();
	pthread_t overlord_thread;
	pthread_create(&overlord_thread,NULL,overlord_handler,NULL);
	world_state = init_game();

	while(n_clients<1)
		sleep(1);//wait for clients to connect

	while(1)
		update_all(&world_state);
}
