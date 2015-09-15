#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

int init_server()
{
	int portnum = 2903;
	struct sockaddr_in address;
	int err = 0;
	int ss = socket(PF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_port = htons(portnum);

	printf("port number:%i\n",portnum);

	err = bind(ss,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(err){
		printf("bind err: %i\n",err);
		exit(-1);
	}else{
		printf("socket binded.\n");
	}

	err = listen(ss,10);
	if(err){
		printf("listen err: %i\n",err);
		exit(-1);
	}else{
		printf("socket listening.\n");
	}

	return ss;
}

/* start a new thread for each new player */
void handle(int player_fd)
{
	printf("TODO handle %i \n",player_fd);
}

void accept_loop(int server_socket)
{
	int np = 0;
	while(1){
		np = accept(server_socket,NULL,NULL);
		handle(np);
	}
}

int main()
{
	int server_socket = init_server();
	accept_loop(server_socket);
	/* TODO main universe loop */
}
