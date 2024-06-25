#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <ncurses.h>

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
		.color=COLOR_ORANGE,
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
		.color=COLOR_GREEN,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','S','S','\0'},
			{'S','S','\0','\0'},
			{'\0','\0','\0','\0'}}
	},
	{
		.name='Z',
		.color=COLOR_RED,
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


struct c_shape current_cshape;
char grid[ROW_GRID][COL_GRID] = {};
int key;
int pre_key;
float delay = 0.004;
clock_t start;

int main(int argc, char** argv) {
	srand(time(NULL));
	setlocale(LC_ALL, "");

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	timeout(300);
	cbreak();

	if (has_colors() != 1) {
		endwin();
		printf("Your terminal does not support color\n");
        exit(1);
	}
	start_color();
	for (int i = 1; i < 8; i++) init_pair(i, SHAPES[i-1].color, 0);

	bkgd(COLOR_PAIR(rand() % SIZE_SHAPE)); // random background color
	current_cshape = insert_shape(grid, pick_shape());
	start = clock();
	while (1) {
		if (((float)(clock() - start) / CLOCKS_PER_SEC) > delay) {
			if (move_down(&current_cshape, grid) == TRUE) {
				current_cshape = insert_shape(grid, pick_shape());
				continue;
			};
			start = clock();
		}
		display_grid(grid);
		refresh();
		key = getch();
		if (pre_key == KEY_DOWN || pre_key == 'W' || pre_key == 'w' &&
			pre_key != KEY_DOWN || pre_key != 'W' || pre_key != 'w')
			delay = 0.004;
		switch (key) {
		case 'w':
		case 'W':
		case KEY_UP:
			shape_rotate_right(&current_cshape, grid);
			break;
		case 's':
		case 'S':
		case KEY_DOWN:
			// speed up
			delay = 0.001;
			break;
		case 'd':
		case 'D':
		case KEY_RIGHT:
			move_right(&current_cshape, grid);
			break;
		case 'a':
		case 'A':
		case KEY_LEFT:
			move_left(&current_cshape, grid);
			break;
		case 'j':
			if (drop_shape(&current_cshape, grid) == TRUE)
				current_cshape = insert_shape(grid, pick_shape());
			break;
		case 'q':
		case 'Q':
			goto endgame;
		}
		pre_key = key;
		clear();
	}

	endgame:
		endwin();
}