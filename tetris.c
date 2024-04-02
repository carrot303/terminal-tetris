#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"

#define MIDDLE_COL (COL_GRID/2-2)


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

struct c_shape insert_shape(char board[ROW_GRID][COL_GRID], struct shape ishape) {
	int cnt = 0;
	struct c_shape ic_shape;
	struct point p;
	ic_shape.ishape = ishape;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (ishape.structure[r][c] != '\0') {
				board[r][c+MIDDLE_COL] = ishape.structure[r][c];
				p.row = r;
				p.col = c+MIDDLE_COL;
				ic_shape.points[cnt++] = p;
			}
		}
	}
	return ic_shape;
}


void move_down(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row++][ic_shape->points[p].col] = '\0';
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
}

void move_right(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row][ic_shape->points[p].col++] = '\0';
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
}

void move_left(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row][ic_shape->points[p].col--] = '\0';
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
}

void move(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID], char action) {
	switch (action) {
	case 'L': move_left(ic_shape, board); break;
	case 'R': move_right(ic_shape, board); break;
	case 'D': move_down(ic_shape, board); break;
	default: break;
	}
}

void point_rotate_right(struct point* ptr) {
	int abs_row = (ptr->row)%4; // TODO: there is a bug here -> when it's divisible by 4 its become 0, but may not be zero?
	int abs_col = (ptr->col)%4; // TODO: there is a bug here -> when it's divisible by 4 its become 0, but may not be zero?
	int final_abs_row = abs_col;
	int final_abs_col = 3-abs_row;
	ptr->row += final_abs_row-abs_row;
	ptr->col += final_abs_col-abs_col;
}

void point_rotate_left(struct point* ptr) {
	int abs_row = (ptr->row)%4; // TODO: there is a bug here -> when it's divisible by 4 its become 0, but may not be zero?
	int abs_col = (ptr->col)%4; // TODO: there is a bug here -> when it's divisible by 4 its become 0, but may not be zero?
	int final_abs_row = 3-abs_col;
	int final_abs_col = abs_row;
	ptr->row += final_abs_row-abs_row;
	ptr->col += final_abs_col-abs_col;
}

void shape_rotate_right(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row][ic_shape->points[p].col] = '\0';
		printf("(%d,%d) -> ", ic_shape->points[p].row,ic_shape->points[p].col);
		point_rotate_right(&ic_shape->points[p]);
		printf("(%d,%d)\n", ic_shape->points[p].row,ic_shape->points[p].col);
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
}

void shape_rotate_left(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row][ic_shape->points[p].col] = '\0';
		printf("(%d,%d) -> ", ic_shape->points[p].row,ic_shape->points[p].col);
		point_rotate_left(&ic_shape->points[p]);
		printf("(%d,%d)\n", ic_shape->points[p].row,ic_shape->points[p].col);
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
}