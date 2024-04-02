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


// Enums
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


// Structures
struct shape {
	char name;
	char structure[4][4];
	int color;
};

struct point {
	int row;
	int col;
};

struct c_shape {
	struct shape ishape;
	struct point points[4];
};


// Globals variables
extern struct shape SHAPES[SIZE_SHAPE];


// Function declarations
void display_grid(char board[ROW_GRID][COL_GRID]);
void display_cell(int r, int c, char board[ROW_GRID][COL_GRID]);
struct shape pick_shape(void);
struct shape get_shape_from_name(char sname);
struct c_shape insert_shape(char board[ROW_GRID][COL_GRID], struct shape);

// Actions function declarations for shapes
void move(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID], char action);
void move_down(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);
void move_right(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);
void move_left(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);
void shape_rotate_right(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);
void shape_rotate_left(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);
void point_rotate_right(struct point* ptr);
void point_rotate_left(struct point* ptr);
void drop(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);
int check_position(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]);