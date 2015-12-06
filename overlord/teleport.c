#include "teleport.h"
#include "servling.h"

int get_server_of_door(int doornum)
{
	int ret = doornum;
	ret -= doornum%10;
	ret /=10;
	return ret;
}

void teleport_player(int player_fd,int doornum)
{
	int servernum = get_server_of_door(doornum);
	printf("teleporting player to servling[%i]\n",servernum);
	add_player_to_servling(player_fd,servling[servernum]);
}

