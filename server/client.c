#include <stdio.h>
#include <unistd.h>
#include "client.h"

int n_clients;
struct client clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex;

void init_clients()
{
	n_clients=0;
	pthread_mutex_init(&clients_mutex,NULL);

	int i;
	for(i=0;i<MAX_CLIENTS;i++){
		clients[i].fd = 0;
		clients[i].pi = (struct player_input){0};
	}
}

void add_client(int fd)
{
	printf("adding client %i\n",fd);
	pthread_mutex_lock(&clients_mutex);
	clients[n_clients].fd = fd;
	n_clients++;
	pthread_mutex_unlock(&clients_mutex);
}
