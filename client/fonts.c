#include <string.h>
#include "fonts.h"
#include "graphics.h"

char * font_letter(char input)
{
	switch(input){
		case 'A':
			return 	" #  "
				"# # "
				"### "
				"# # "
				"# # "
				"    ";
		case 'B':
			return 	"##  "
				"# # "
				"##  "
				"# # "
				"##  "
				"    ";
		case 'C':
			return 	" ## "
				"#   "
				"#   "
				"#   "
				" ## "
				"    ";
		case 'D':
			return 	"##  "
				"# # "
				"# # "
				"# # "
				"##  "
				"    ";
		case 'E':
			return 	"### "
				"#   "
				"### "
				"#   "
				"### "
				"    ";
		case 'F':
			return 	"### "
				"#   "
				"### "
				"#   "
				"#   "
				"    ";
		case 'G':
			return 	" ## "
				"#   "
				"### "
				"# # "
				" ## "
				"    ";
		case 'H':
			return 	"# # "
				"# # "
				"### "
				"# # "
				"# # "
				"    ";
		case 'I':
			return 	"### "
				" #  "
				" #  "
				" #  "
				"### "
				"    ";
		case 'L':
			return 	"#   "
				"#   "
				"#   "
				"#   "
				"### "
				"    ";
		case 'M':
			return 	"# # "
				"### "
				"### "
				"# # "
				"# # "
				"    ";
		case 'N':
			return 	"# # "
				"### "
				"### "
				"### "
				"# # "
				"    ";
		case 'O':
			return 	" #  "
				"# # "
				"# # "
				"# # "
				" #  "
				"    ";
	}
	return 	"### "
		"  # "
		" ## "
		" #  "
		"    "
		" #  ";
}

void draw_text(double x, double y, char * text)
{
	int i;
	int l = strlen(text);
	for(i=0;i<l;i++)
	{
		draw_letter(x,y,font_letter(text[i]));
		x-=FONT_WIDTH;
	}
}
