#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <ncurses.h>

#include "tetris.h"
#include "game.h"


WINDOW* game_win, *preview_shape_win, *score_win;

struct shape SHAPES[SIZE_SHAPE] = {
	{
		.name='O',
		.color=COLOR_YELLOW,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','O','O','\0'},
			{'\0','O','O','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=1
	},
	{
		.name='L',
		.color=COLOR_ORANGE,
		.structure=
			{{'\0','L','\0','\0'},
			{'\0','L','\0','\0'},
			{'\0','L','L','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=1
	},
	{
		.name='J',
		.color=COLOR_BLUE,
		.structure=
			{{'\0','J','\0','\0'},
			{'\0','J','\0','\0'},
			{'J','J','\0','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	},
	{
		.name='I',
		.color=COLOR_CYAN,
		.structure=
			{{'\0','\0','I','\0'},
			{'\0','\0','I','\0'},
			{'\0','\0','I','\0'},
			{'\0','\0','I','\0'}},
		.init_position=2
	},
	{
		.name='S',
		.color=COLOR_GREEN,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','S','S','\0'},
			{'S','S','\0','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	},
	{
		.name='Z',
		.color=COLOR_RED,
		.structure=
			{{'\0','\0','\0','\0'},
			{'Z','Z','\0','\0'},
			{'\0','Z','Z', '\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	},
	{
		.name='T',
		.color=COLOR_MAGENTA,
		.structure=
			{{'\0','\0','\0','\0'},
			{'T','T','T','\0'},
			{'\0','T','\0','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	}
};

/* specify origin rule:
	per shape there is rule to find out the origin point, e.g:
	if shape is O_SHAPE and current direction of shape is UP,
	rule for origin point is (-1, -1): that means the first point row,col must + with (-1, -1)
*/
int ORIGIN_RULES[SIZE_SHAPE][4][2] = {
	{{-1, -1}, 	{-1, -1}, 	{-1, -1}, 	{-1, -1}}, 		// O_SHAPE
	{{0, -1}, 	{-1, -1}, 	{-1, -1}, 	{-1, -2}}, 		// L_SHAPE
	{{0, -1}, 	{0, -1}, 	{-1, -2}, 	{-2, 0}}, 		// J_SHAPE
	{{0, -2}, 	{-2, 0}, 	{0, -1}, 	{-1, 0}}, 		// I_SHAPE
	{{-1, -1}, 	{0, -1}, 	{-1, -2}, 	{-1, -1}}, 		// S_SHAPE
	{{-1, 0}, 	{0, -2}, 	{-1, -1}, 	{-1, -2}}, 		// Z_SHAPE
	{{-1, 0}, 	{0, -2}, 	{-1, -2}, 	{-1, -1}}, 		// T_SHAPE
};


struct shape next_shape;
char board[ROW_GRID][COL_GRID] = {};
struct c_shape current_cshape;
int losed = FALSE;
int score = 0;


int main(int argc, char** argv) {
	srand(time(NULL));
	setlocale(LC_ALL, "");

	init_windows();
	loop();
}