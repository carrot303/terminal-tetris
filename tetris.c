#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"

#define MIDDLE_COL ((COL_GRID/2-2)-1)
#define ABS(x) (x >= 0 ? x : -x)


void display_cell(int r, int c, char board[ROW_GRID][COL_GRID]) {
	if (r == INIT_ROW && c == INIT_COL)
		addstr(CHAR_TOP_LEFT_CORNER);
	else if (r == INIT_ROW && c == COL_GRID)
		addstr(CHAR_TOP_RIGHT_CORNER);
	else if (r == ROW_GRID && c == INIT_COL)
		addstr(CHAR_BOTTOM_LEFT_CORNER);
	else if (r == ROW_GRID && c == COL_GRID)
		addstr(CHAR_BOTTOM_RIGHT_CORNER);
	else if ((r == INIT_ROW || r == ROW_GRID) && INIT_COL < c < COL_GRID) {
		addstr(CHAR_HORIZANTAL_EDGE);
		addstr(CHAR_HORIZANTAL_EDGE);
	}
	else if (c == INIT_COL || c == COL_GRID)
		addstr(CHAR_VERTICAL_EDGE);
	else {
		if (board[r][c]) {
			attron(COLOR_PAIR(get_shape_index(board[r][c])+1));
			printw(BLOCK);
			attroff(COLOR_PAIR(get_shape_index(board[r][c])+1));
		}
		else
			addstr("  ");
	}
}

void display_grid(char board[ROW_GRID][COL_GRID]) {
	for (int r = INIT_ROW; r <= ROW_GRID; r++) {
		for (int c = INIT_COL; c <= COL_GRID; c++) {
			display_cell(r, c, board);
		}
		printw("\n");
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
	ic_shape.direction = 0;
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

void move_up(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row--][ic_shape->points[p].col] = '\0';
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

void do_action(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID], char action) {
	switch (action) {
	case 'L': move_left(ic_shape, board); break;
	case 'R': move_right(ic_shape, board); break;
	case 'D': move_down(ic_shape, board); break;
	case 'U': move_up(ic_shape, board); break;
	case '>': shape_rotate_right(ic_shape, board); break;
	case '<': shape_rotate_right(ic_shape, board); break;
	default: break;
	}
}

void point_rotate_right(struct point* ptr, struct point* origin_point) {
	int abs_row = ptr->row-origin_point->row;
	int abs_col = ptr->col-origin_point->col;
	int final_abs_row = abs_col;
	int final_abs_col = 3-abs_row;
	ptr->row += final_abs_row-abs_row;
	ptr->col += final_abs_col-abs_col;
}

void point_rotate_left(struct point* ptr, struct point* origin_point) {
	int abs_row = ptr->row-origin_point->row;
	int abs_col = ptr->col-origin_point->col;
	int final_abs_row = 3-abs_col;
	int final_abs_col = abs_row;
	ptr->row += final_abs_row-abs_row;
	ptr->col += final_abs_col-abs_col;
}

void shape_rotate_right(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	struct point origin_point = get_origin_point(ic_shape);
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row][ic_shape->points[p].col] = '\0';
		point_rotate_right(&ic_shape->points[p], &origin_point);
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
	ic_shape->direction++;
}

void shape_rotate_left(struct c_shape* ic_shape, char board[ROW_GRID][COL_GRID]) {
	struct point tmp_pts[4];
	struct point origin_point = get_origin_point(ic_shape);
	for (int p = 0; p < 4; p++) {
		board[ic_shape->points[p].row][ic_shape->points[p].col] = '\0';
		point_rotate_left(&ic_shape->points[p], &origin_point);
		tmp_pts[p] = ic_shape->points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = ic_shape->ishape.name;
	ic_shape->direction += 3;
}

int get_shape_index(char shape_name) {
	int shape_idx;
	switch (shape_name) {
	case 'O': shape_idx = O_SHAPE; break;
	case 'L': shape_idx = L_SHAPE; break;
	case 'J': shape_idx = J_SHAPE; break;
	case 'I': shape_idx = I_SHAPE; break;
	case 'S': shape_idx = S_SHAPE; break;
	case 'Z': shape_idx = Z_SHAPE; break;
	case 'T': shape_idx = T_SHAPE; break;
	default: break;
	}
	return shape_idx;
}

struct point get_origin_point(struct c_shape* ic_shape) {
	int cnt, sum, idx, rmin = COL_GRID*ROW_GRID+1, cmin = COL_GRID*ROW_GRID+1;
	int abs_direction;
	struct point origin_point;
	int topest_points[4] = {0, 0, 0, 0};
	for (int i = 0, cnt = 0; i < 4; i++)
		rmin = ic_shape->points[i].row < rmin ? ic_shape->points[i].row : rmin;
	for (int i = 0, cnt = 0; i < 4; i++)
		topest_points[i] = ic_shape->points[i].row == rmin ? 1 : 0;
	for (int i = 0, cnt = 0; i < 4; i++) {
		if (topest_points[i] == 1 && ic_shape->points[i].col < cmin) {
			cmin = ic_shape->points[i].col;
			idx = i;
		}
	}
	origin_point = ic_shape->points[idx];
	abs_direction = ic_shape->direction%4;
	origin_point.row += ORIGIN_RULES[get_shape_index(ic_shape->ishape.name)][abs_direction][0];
	origin_point.col += ORIGIN_RULES[get_shape_index(ic_shape->ishape.name)][abs_direction][1];
	return origin_point;
}