#ifndef CLIENT
#define CLIENT

#include <poly.h>
#include <pthread.h>

struct client {
	int fd;
	struct vector location;
};

#define MAX_CLIENTS 10

extern pthread_mutex_t clients_mutex;

void init_clients();
void add_client(int fd);
 
#endif
