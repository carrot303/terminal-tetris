#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"

struct shape get_shape_from_name(char sname) {
	switch (sname) {
	case 'O': return SHAPES[O_SHAPE]; break;
	case 'L': return SHAPES[L_SHAPE]; break;
	case 'J': return SHAPES[J_SHAPE]; break;
	case 'I': return SHAPES[I_SHAPE]; break;
	case 'S': return SHAPES[S_SHAPE]; break;
	case 'Z': return SHAPES[Z_SHAPE]; break;
	case 'T': return SHAPES[T_SHAPE]; break;
	default:
		struct shape s = {};
		return s;
		break;
	}
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
			printf("\e[%dm%s\e[0m", get_shape_from_name(board[r][c]).color, BLOCK);
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

struct shape pick_shape(void) {
	return SHAPES[rand() % SIZE_SHAPE];
}

int insert_shape(char board[ROW_GRID][COL_GRID], struct shape ishape) {
	for (int r = 0; r < 4; r++) {
		memcpy(board[r]+(COL_GRID/2)-1, ishape.structure[r], 4);
	}
}