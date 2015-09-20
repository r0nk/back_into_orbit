#ifndef NETWORKING
#define NETWORKING

#include <protolol.h>

int init_networking();

/* tell the server what state we think we're in, then update ourselves to 
   match what state the server tells us we're in. */
struct game_state update_state(int server_fd, struct game_state gs);
 
#endif
