#include <ncurses.h>

#define SCORE_PER_SHAPE 10

extern struct shape SHAPES[SIZE_SHAPE];
extern int ORIGIN_RULES[SIZE_SHAPE][4][2];
extern int losed;
extern int score;
extern char board[ROW_GRID][COL_GRID];
extern WINDOW* game_win, *preview_shape_win, *score_win;


void init_windows();
void loop();