#ifndef CLIENT
#define CLIENT

#include <poly.h>
#include <input.h>

#include <pthread.h>

struct client {
	int fd;
	struct player_input pi;
};

#define MAX_CLIENTS 10

extern pthread_mutex_t clients_mutex;

void init_clients();
void add_client(int fd);

extern struct client clients[MAX_CLIENTS];
extern int n_clients;
 
#endif
