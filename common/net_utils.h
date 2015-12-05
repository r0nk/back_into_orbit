#ifndef NET_UTILS
#define NET_UTILS

static inline int init_server(int port)
{
	struct sockaddr_in address;
	int e = 0;
	int ss = socket(PF_INET,SOCK_STREAM,0);
	int optval = 1;
	setsockopt(ss,SOL_SOCKET,SO_REUSEPORT,&optval,sizeof(optval));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr ("127.0.0.1");
	address.sin_port = htons(port);

	e = bind(ss,(struct sockaddr *)&address,sizeof(struct sockaddr_in));
	if(e)
		err(2,"init_server() binding");

	e = listen(ss,10);
	if(e)
		err(3,"init_server() listen");
	return ss;
}

static inline int connect_to_overlord(int port)
{
	struct sockaddr_in address;
	int cs = socket(PF_INET,SOCK_STREAM,0);
	int e = 0;

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_aton("127.0.0.1",&address.sin_addr);

	e = connect(cs,(struct sockaddr *)&address,sizeof(struct sockaddr_in));

	if(e)
		err(1,"connect_to_overlord()");

	return cs;
}
 
#endif
