#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(){
	struct sockaddr address;
	printf("hello world! \n");
	int s = socket(PF_INET,SOCK_STREAM,0);

	int err = 0;
	err = listen(s,10);
	if(err){
		printf("listen err \n");
		exit(-1);
	}

	//TODO: bind and accept

	printf("s:%i \n",s);
	return 0;
}

