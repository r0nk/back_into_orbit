#ifndef PROTOLOL
#define PROTOLOL

/* the networking protocol both the server and the client share */

struct protolol_packet {
	char magic_start[4];// should be "oHai"
	char type;// whether or not its a game state or kick or whatever
	int length;//in bytes, this header included.
}
 
#endif
