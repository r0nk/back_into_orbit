#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <pthread.h>

#include <protolol.h>

int server_socket;

void handle_player_login()
{
	/*open socket()*/
	/* check credentials for player*/
	//figure out the right server for the player
	//if theres no servling
		// right_server = start_servling();
	//add_player_to_servling(p,right_server);
}

void login_loop()
{
	while(1)
		handle_player_login();
}

void accept_loop()
{
	int np = 0;
	while(1){
		np = accept(server_socket,NULL,NULL);
		printf("overlord accepted connection: %i\n",np);
	}
}

void start_servling()
{
	if(!fork())
		execl("../server/dr0ne_server","127.0.0.1");
}

void transfer_player()
{
	//TODO: make the magic happen in here.
	// tell s1(players current server) to drop player
	//add_player_to_servling(p,s2);
}

/*
void add_player_to_servling(struct player * p, struct servling * s)
{
	// tell servling its about to get a player p
	// tell the player to connect to the servling s
}
*/

int init_server()
{
	struct sockaddr_in address;
	int e = 0;
	int ss = socket(PF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr ("127.0.0.1");
	address.sin_port = htons(PROTOLOL_OVER_PORT);

	e = bind(ss,(struct sockaddr *)&address,sizeof(struct sockaddr_in));
	if(e)
		err(2,"init_server() binding");

	e = listen(ss,10);
	if(e)
		err(3,"init_server() listen");
	return ss;
}

int main()
{
	server_socket=init_server();
	start_servling();
	pthread_t accept_thread;
	pthread_create(&accept_thread,NULL,accept_loop,NULL);
	while(1)
		sleep(1);
}
