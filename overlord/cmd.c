#include <stdio.h>
#include <string.h>

#include "cmd.h"
#include "servling.h"

int do_cmd(char * cmd)
{
	if(!strcmp(cmd,"exit"))
		return 0;
	if(!strcmp(cmd,"status"))
		printf("n_servlings:%i\n",n_servlings);
	return 1;
}

