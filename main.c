#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "tetris.h"


struct shape SHAPES[SIZE_SHAPE] = {
	{.name='O', .color=YELLOW, .structure={{'O','O','\0','\0'},
										   {'O','O','\0','\0'},
										   {'\0','\0','\0','\0'},
										   {'\0','\0','\0','\0'}}},

	{.name='L', .color=WHITE, .structure={{'L','\0','\0','\0'},
										  {'L','\0','\0','\0'},
										  {'L','L','\0','\0'},
										  {'\0','\0','\0','\0'}}},

	{.name='J', .color=BLUE, .structure={{'\0','J','\0','\0'},
										 {'\0','J','\0','\0'},
										 {'J','J','\0','\0'},
										 {'\0','\0','\0','\0'}}},

	{.name='I', .color=CYAN, .structure={{'I','\0','\0','\0'},
										 {'I','\0','\0','\0'},
										 {'I','\0','\0','\0'},
										 {'I','\0','\0','\0'}}},

	{.name='S', .color=RED, .structure={{'\0','S','S','\0'},
										{'S','S','\0','\0'},
										{'\0','\0','\0','\0'},
										{'\0','\0','\0','\0'}}},

	{.name='Z', .color=GREEN, .structure={{'Z','Z','\0','\0'},
										  {'\0','Z','Z', '\0'},
										  {'\0','\0','\0','\0'},
										  {'\0','\0','\0','\0'}}},

	{.name='T', .color=MAGENTA, .structure={{'T','T','T','\0'},
											{'\0','T','\0','\0'},
											{'\0','\0','\0','\0'},
											{'\0','\0','\0','\0'}}}
};


int main(int argc, char** argv) {
	srand(time(NULL));
	char grid[ROW_GRID][COL_GRID] = {};
	insert_shape(grid, pick_shape());
	display_grid(grid);
}