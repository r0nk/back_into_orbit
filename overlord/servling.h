#ifndef SERVLING
#define SERVLING 1
#include <pthread.h>
 
struct servling {
	char * ascii_ip; // i.e. "127.0.0.1" 
	int fd;
	pthread_t thread;
};

#define MAX_SERVLINGS 50
int n_servlings;
struct servling servling[MAX_SERVLINGS];

void add_servling(int fd);
void start_servling();

#endif
