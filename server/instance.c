#include "instance.h"


struct instance mkinstance(char * room_name)
{
	struct instance inst;
	inst.room = mkroom(room_name);
	return inst;
}
