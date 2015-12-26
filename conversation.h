#ifndef CONVERSATION
#define CONVERSATION

struct line {
	char speaker; /*'a' or 'b'*/
	double duration; /* how long the unit is going to say the line */
	char words[80];/* the actually words the unit is going to say */
};

struct conversation
{
	struct unit * a,b;
	char lines[24][80];
};
 
#endif
