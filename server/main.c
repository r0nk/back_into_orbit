#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

int server_socket;

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
	sleep(1);
}

void accept_loop()
{
	int np = 0;
	while(1){
		np = accept(server_socket,NULL,NULL);
		handle(np);
	}
}

int main()
{
	server_socket = init_server();
	pthread_t accept_thread;
	pthread_create(&accept_thread,NULL,accept_loop,NULL);
	while(1){
		printf("abcdefg\n");
		sleep(1);
	}
	//accept_loop(server_socket);
	/* TODO main universe loop */
}
