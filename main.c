#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>

#include "tetris.h"


struct shape SHAPES[SIZE_SHAPE] = {
	{
		.name='O',
		.color=COLOR_YELLOW,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','O','O','\0'},
			{'\0','O','O','\0'},
			{'\0','\0','\0','\0'}}
	},
	{
		.name='L',
		.color=COLOR_WHITE,
		.structure=
			{{'\0','L','\0','\0'},
			{'\0','L','\0','\0'},
			{'\0','L','L','\0'},
			{'\0','\0','\0','\0'}}
	},
	{
		.name='J',
		.color=COLOR_BLUE,
		.structure=
			{{'\0','J','\0','\0'},
			{'\0','J','\0','\0'},
			{'J','J','\0','\0'},
			{'\0','\0','\0','\0'}}
	},
	{
		.name='I',
		.color=COLOR_CYAN,
		.structure=
			{{'\0','\0','I','\0'},
			{'\0','\0','I','\0'},
			{'\0','\0','I','\0'},
			{'\0','\0','I','\0'}}
	},
	{
		.name='S',
		.color=COLOR_RED,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','S','S','\0'},
			{'S','S','\0','\0'},
			{'\0','\0','\0','\0'}}
	},
	{
		.name='Z',
		.color=COLOR_GREEN,
		.structure=
			{{'\0','\0','\0','\0'},
			{'Z','Z','\0','\0'},
			{'\0','Z','Z', '\0'},
			{'\0','\0','\0','\0'}}
	},
	{
		.name='T',
		.color=COLOR_MAGENTA,
		.structure=
			{{'\0','\0','\0','\0'},
			{'T','T','T','\0'},
			{'\0','T','\0','\0'},
			{'\0','\0','\0','\0'}}
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

int main(int argc, char** argv) {
	struct c_shape current_cshape;
	char grid[ROW_GRID][COL_GRID] = {};
	srand(time(NULL));
	setlocale(LC_ALL, "");

	initscr();
	if (has_colors() != 1) {
		endwin();
		printf("Your terminal does not support color\n");
        exit(1);
	}
	start_color();
	// init color pairs
	for (int i = 1; i < 8; i++) {
		init_pair(i, SHAPES[i-1].color, 0);
	}

	current_cshape = insert_shape(grid, pick_shape());
	while (1) {
		display_grid(grid);
		refresh();
		getch();
		clear();
	}

	endwin();
}