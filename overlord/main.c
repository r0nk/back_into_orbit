#include <stdio.h>

void login_loop()
{
	while(1)
		handle_player_login();
}

void start_servling()
{
	// fork, exec, yada yada
}

void handle_player_login()
{
	//TODO: make the magic happen in here.
	/*open socket*/
	/* check credentials for player*/
	//figure out the right server for the player
	//if theres no servling
		// right_server = start_servling();
	//add_player_to_servling(p,right_server);
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

int main()
{
	printf("placeholder hello world!\n");
	/* start login thread */
}
