#include <stdio.h>
#include <string.h>

#include "cmd.h"
#include "map.h"
#include "servling.h"

int do_cmd(char * cmd)
{
	if(!strcmp(cmd,"exit"))
		return 0;
	if(!strcmp(cmd,"status"))
		printf("n_servlings:%i\n",n_servlings);
	if(!strcmp(cmd,"map"))
		dump_map(overmap);
	return 1;
}
