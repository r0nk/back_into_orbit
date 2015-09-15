#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(){
	int portnum = 2903;
	struct sockaddr_in address;
	int err = 0;
	int sock = 0;// for each client
	int server_socket = socket(PF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_port = htons(portnum);

	printf("port number:%i\n",portnum);


	err = bind(server_socket,(struct sockaddr *)&address,
			sizeof(struct sockaddr_in));
	if(err){
		printf("bind err: %i\n",err);
		exit(-1);
	}else{
		printf("socket binded.\n");
	}


	err = listen(server_socket,10);
	if(err){
		printf("listen err: %i\n",err);
		exit(-1);
	}else{
		printf("socket listening.\n");
	}

	sock = accept(server_socket,NULL,NULL);

	printf("server_socket:%i \n",server_socket);
	printf("sock:%i \n",server_socket);
	return 0;
}

