#ifndef CONVERSATION
#define CONVERSATION

struct line {
	char speaker; /*'a' or 'b'*/
	double duration; /* how long the unit is going to say the line */
	char words[80];/* the actually words the unit is going to say */
};

void dump_line(struct line line)
{
	printf("line speaker: %c\n",line.speaker);
	printf("line duration: %i\n",line.duration);
	printf("\"%s\"\n",line.words);
}

struct conversation
{
	struct unit * a,b;
	int n_lines;
	struct line line[24];
};

void dump_conversation(struct conversation convo)
{
	printf("conversation between %p and %p",convo.a,convo.b);
	printf("n_lines:%i",convo.n_lines);
	int i;
	for(i=0;i<10;i++){
		printf("line[%i]:\n",i);
		dump_line(line[i]);
	}
}
 
#endif
