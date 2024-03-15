#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"

int shape_colour(char shape) {
	int colour_code;
	switch (shape) {
	case 'O': colour_code = 31; break;
	case 'L': colour_code = 32; break;
	case 'J': colour_code = 33; break;
	case 'I': colour_code = 36; break;
	case 'S': colour_code = 35; break;
	case 'Z': colour_code = 36; break;
	case 'A': colour_code = 33; break;
	default: colour_code = 0; break;
	}
	return colour_code;
}

void display_cell(int r, int c, char board[ROW_GRID][COL_GRID]) {
	if (r == INIT_ROW && c == INIT_COL)
		printf("%s", CHAR_TOP_LEFT_CORNER);
	else if (r == INIT_ROW && c == COL_GRID)
		printf("%s", CHAR_TOP_RIGHT_CORNER);
	else if (r == ROW_GRID && c == INIT_COL)
		printf("%s", CHAR_BOTTOM_LEFT_CORNER);
	else if (r == ROW_GRID && c == COL_GRID)
		printf("%s", CHAR_BOTTOM_RIGHT_CORNER);
	else if ((r == INIT_ROW || r == ROW_GRID) && INIT_COL < c < COL_GRID)
		printf("%s%s", CHAR_HORIZANTAL_EDGE, CHAR_HORIZANTAL_EDGE);
	else if (c == INIT_COL || c == COL_GRID)
		printf("%s", CHAR_VERTICAL_EDGE);
	else {
		if (board[r][c])
			printf("\e[%dm%s\e[0m", shape_colour(board[r][c]), BLOCK);
		else
			printf("  ");
	}
}

void display_grid(char board[ROW_GRID][COL_GRID]) {
	for (int r = INIT_ROW; r <= ROW_GRID; r++) {
		for (int c = INIT_COL; c <= COL_GRID; c++) {
			display_cell(r, c, board);
		}
		printf("\n");
	}
}


int main(int argc, char* argv[]) {
	char grid[ROW_GRID][COL_GRID] = {};
	display_grid(grid);
}