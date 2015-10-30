#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include "networking.h"

/* Tell the server what state we think we're in, then update ourselves to 
 * match what state the server tells us we're in. */
struct game_state update_state(int server_fd, struct game_state gs,
		struct player_input pi)
{
	send_player_input(pi,server_fd);
	gs = recv_game_state(server_fd);
	return gs;
}

/* Get connected to the server, and return the server file descriptor. */
int init_networking()
{
	struct sockaddr_in address;
	int cs = socket(PF_INET,SOCK_STREAM,0);
	int e = 0;

	address.sin_family = AF_INET;
	address.sin_port = htons(PROTOLOL_PORT);
	inet_aton("127.0.0.1",&address.sin_addr);

	e = connect(cs,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(e)
		err(1,"init_networking()");

	return cs;
}

