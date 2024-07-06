#include <ncurses.h>

#define SCORE_PER_SHAPE 10
#define LEVEL_UP_POINT 1000

extern struct shape SHAPES[SIZE_SHAPE];
extern int ORIGIN_RULES[SIZE_SHAPE][4][2];
extern int losed;
extern int score;
extern char board[ROW_GRID][COL_GRID];
extern WINDOW* game_win, *preview_shape_win, *score_win, *hint_win, *prompt_win;


void init_windows();
void loop();
void preview_shape();
void show_score();
void update_screen();
void display_hints();
int destroy_game();
void write_text(WINDOW*, char*, int);
void pause_game();
void reset_game();
int prompt_user(char*, int);
void endgame();
