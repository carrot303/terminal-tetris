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

enum Shapes {
	O_SHAPE,
	L_SHAPE,
	J_SHAPE,
	I_SHAPE,
	S_SHAPE,
	Z_SHAPE,
	T_SHAPE,
	SIZE_SHAPE
};

enum Colors {BLACK=30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};

struct shape {
	char name;
	char structure[4][4];
	int color;
};

extern struct shape SHAPES[SIZE_SHAPE];

void display_grid(char board[ROW_GRID][COL_GRID]);
void display_cell(int r, int c, char board[ROW_GRID][COL_GRID]);
struct shape pick_shape(void);
struct shape get_shape_from_name(char sname);
int insert_shape(char board[ROW_GRID][COL_GRID], struct shape);