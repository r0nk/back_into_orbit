#ifndef SERVLING
#define SERVLING
 
struct servling {
	char * ascii_ip; // i.e. "127.0.0.1" 
	int fd;
};

#define MAX_SERVLINGS 50
int n_servlings;
struct servling servling[MAX_SERVLINGS];

static inline void add_servling(int fd)
{
	if(n_servlings>=MAX_SERVLINGS)
		return;
	n_servlings++;
	servling[n_servlings].fd=fd;
	servling[n_servlings].ascii_ip="127.0.0.1";
}

#endif
