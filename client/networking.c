#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include "networking.h"

int init_networking(){
	struct sockaddr_in address;
	int cs = socket(PF_INET,SOCK_STREAM,0);
	int err=0;

	printf("initalizing networking\n");

	address.sin_family = AF_INET;
	address.sin_port = htons(PROTOLOL_PORT);
	inet_aton("127.0.0.1",&address.sin_addr);

	err=connect(cs,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(err){
		printf("net err : %i\n",err);
		printf("errno : %i\n",errno);
	}

	return 0;
}

