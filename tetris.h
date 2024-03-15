/*
 * Tetris header file
 *	include some macros, declaration of structs and functions
 */


#define CHAR_TOP_RIGHT_CORNER 		"\xE2\x94\x90"
#define CHAR_TOP_LEFT_CORNER 		"\xE2\x94\x8C"
#define CHAR_BOTTOM_LEFT_CORNER 	"\xE2\x94\x94"
#define CHAR_BOTTOM_RIGHT_CORNER	"\xE2\x94\x98"
#define CHAR_VERTICAL_EDGE 			"\xE2\x94\x82"
#define CHAR_HORIZANTAL_EDGE 		"\xE2\x94\x80"
#define BLOCK						"\xE2\x96\x88\xE2\x96\x88"

#define ROW_GRID 30
#define COL_GRID 20
#define INIT_ROW -1
#define INIT_COL -1
#define S_SHAPE 7

char SHAPES[S_SHAPE] = {'O', 'L', 'J', 'I', 'S', 'Z', 'A'};

void display_grid(char board[ROW_GRID][COL_GRID]);
void display_cell(int r, int c, char board[ROW_GRID][COL_GRID]);
int shape_colour(char shape);
char pick_shape(void);