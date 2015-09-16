#include <stdio.h>
#include <err.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include "networking.h"

int init_networking(){
	struct sockaddr_in address;
	int cs = socket(PF_INET,SOCK_STREAM,0);
	int e=0;

	address.sin_family = AF_INET;
	address.sin_port = htons(PROTOLOL_PORT);
	inet_aton("127.0.0.1",&address.sin_addr);

	e=connect(cs,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(e){
		err(1,"init_networking()");
	}

	return 0;
}

