#ifndef FONTS
#define FONTS

/* some credit for the font goes to "Tom Thumb" */
/* http://robey.lag.net/2010/01/23/tiny-monospace-font.html */

/* currently the font size is locked at 4x6 */
#define FONT_X_BITS 4
#define FONT_Y_BITS 6

#define FONT_BIT_WIDTH 0.1
#define FONT_BIT_HEIGHT 0.1

#define FONT_WIDTH 0.4
#define FONT_HEIGHT 0.6

void draw_text(double x, double y, char * text);

#endif
