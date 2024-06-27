#include "tetrominoes.h"
#include "game.h"

extern struct c_shape current_cshape;

int move_down() {
	struct point tmp_pts[4];
	int colission = 0;
	int frow, fcol;
	for (int p = 0; p < 4; p++) {
		frow = current_cshape.points[p].row+1;
		fcol = current_cshape.points[p].col;
		if ((current_cshape.points[p].row == ROW_GRID-1) ||
			(COLLISION_OTHER_SHAPE(frow, fcol, &current_cshape)))
			colission = 1;
	}
	if (colission)
		return TRUE;
	for (int p = 0; p < 4; p++) {
		board[current_cshape.points[p].row++][current_cshape.points[p].col] = '\0';
		tmp_pts[p] = current_cshape.points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = current_cshape.ishape.name;
	return FALSE;
}

void move_right() {
	struct point tmp_pts[4];
	int colission = 0;
	int frow, fcol;
	for (int p = 0; p < 4; p++) {
		frow = current_cshape.points[p].row;
		fcol = current_cshape.points[p].col+1;
		if ((current_cshape.points[p].col == COL_GRID-1) ||
			(COLLISION_OTHER_SHAPE(frow, fcol, &current_cshape)))
			colission = 1;
	}
	if (!colission) {
		for (int p = 0; p < 4; p++) {
			board[current_cshape.points[p].row][current_cshape.points[p].col++] = '\0';
			tmp_pts[p] = current_cshape.points[p];
		}
		for (int p = 0; p < 4; p++)
			board[tmp_pts[p].row][tmp_pts[p].col] = current_cshape.ishape.name;
	}
}

void move_left() {
	struct point tmp_pts[4];
	int colission = 0;
	int frow, fcol;
	for (int p = 0; p < 4; p++) {
		frow = current_cshape.points[p].row;
		fcol = current_cshape.points[p].col-1;
		if ((current_cshape.points[p].col == 0) ||
			(COLLISION_OTHER_SHAPE(frow, fcol, &current_cshape)))
			colission = 1;
	}
	if (!colission) {
		for (int p = 0; p < 4; p++) {
			board[current_cshape.points[p].row][current_cshape.points[p].col--] = '\0';
			tmp_pts[p] = current_cshape.points[p];
		}
		for (int p = 0; p < 4; p++)
			board[tmp_pts[p].row][tmp_pts[p].col] = current_cshape.ishape.name;
	}
}

int point_rotate_right(struct point* ptr, struct point* origin_point, int update) {
	int abs_row = ptr->row-origin_point->row;
	int abs_col = ptr->col-origin_point->col;
	int final_abs_row = abs_col;
	int final_abs_col = 3-abs_row;
	int fcol, frow;
	fcol = ptr->col + final_abs_col-abs_col;
	frow = ptr->row + final_abs_row-abs_row;
	if (frow >= ROW_GRID || frow < 0)
		return TRUE;
	else if (fcol >= COL_GRID || fcol < 0)
		return TRUE;
	else if (COLLISION_OTHER_SHAPE(frow, fcol, &current_cshape))
		return TRUE;

	if (update) {
		ptr->row = frow;
		ptr->col = fcol;
	}
	return FALSE;
}

int point_rotate_left(struct point* ptr, struct point* origin_point, int update) {
	int abs_row = ptr->row-origin_point->row;
	int abs_col = ptr->col-origin_point->col;
	int final_abs_row = 3-abs_col;
	int final_abs_col = abs_row;
	int fcol, frow;
	fcol = ptr->col + final_abs_col-abs_col;
	frow = ptr->row + final_abs_row-abs_row;
	if (frow >= ROW_GRID || frow < 0)
		return TRUE;
	else if (fcol >= COL_GRID || fcol < 0)
		return TRUE;
	else if (COLLISION_OTHER_SHAPE(frow, fcol, &current_cshape))
		return TRUE;

	if (update) {
		ptr->row = frow;
		ptr->col = fcol;
	}
	return FALSE;
}

int shape_rotate_right() {
	struct point tmp_pts[4];
	struct point origin_point = get_origin_point(&current_cshape);
	int colission = 0;
	for (int p = 0; p < 4; p++)
		colission |= point_rotate_right(&current_cshape.points[p], &origin_point, FALSE);

	if (colission) return TRUE;
	for (int p = 0; p < 4; p++) {
		board[current_cshape.points[p].row][current_cshape.points[p].col] = '\0';
		point_rotate_right(&current_cshape.points[p], &origin_point, TRUE);
		tmp_pts[p] = current_cshape.points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = current_cshape.ishape.name;
	current_cshape.direction++;
	return FALSE;
}

int shape_rotate_left() {
	struct point tmp_pts[4];
	struct point origin_point = get_origin_point(&current_cshape);
	int colission = 0;
	for (int p = 0; p < 4; p++)
		colission |= point_rotate_right(&current_cshape.points[p], &origin_point, FALSE);

	if (colission)
		return TRUE;
	for (int p = 0; p < 4; p++) {
		board[current_cshape.points[p].row][current_cshape.points[p].col] = '\0';
		point_rotate_left(&current_cshape.points[p], &origin_point, TRUE);
		tmp_pts[p] = current_cshape.points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = current_cshape.ishape.name;
	current_cshape.direction += 3;
	return FALSE;
}

struct point get_origin_point(struct c_shape* ic_shape) {
	int cnt, sum, shape_index, idx, rmin = COL_GRID*ROW_GRID+1, cmin = COL_GRID*ROW_GRID+1;
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
	shape_index = SHAPE_INDEX(ic_shape->ishape.name);
	origin_point.row += ORIGIN_RULES[shape_index][abs_direction][0];
	origin_point.col += ORIGIN_RULES[shape_index][abs_direction][1];
	return origin_point;
}

int is_point_shape(int row, int col, struct c_shape* ic_shape) {
	for (int i = 0; i < 4; i++) {
		if (row == ic_shape->points[i].row &&
			col == ic_shape->points[i].col)
			return TRUE;
	}
	return FALSE;
}

void drop_shape() {
	struct point tmp_pts[4];
	int frow, fcol;
	int rmin = 0;
	int drop = ROW_GRID;
	int r;
	for (int i = 0; i < 4; i++) {
		if (current_cshape.points[i].row > rmin)
			rmin = current_cshape.points[i].row;
	}

	for (int i = 0; i < 4; i++) {
		r = 0;
		while (1) {
			frow = r+current_cshape.points[i].row;
			fcol = current_cshape.points[i].col;
			if (frow >= ROW_GRID)
				break;
			if (COLLISION_OTHER_SHAPE(frow, fcol, &current_cshape))
				break;
			r++;
		}
		drop = r < drop ? r : drop;
	}
	for (int p = 0; p < 4; p++) {
		board[current_cshape.points[p].row][current_cshape.points[p].col] = '\0';
		current_cshape.points[p].row += drop-1;
		tmp_pts[p] = current_cshape.points[p];
	}
	for (int p = 0; p < 4; p++)
		board[tmp_pts[p].row][tmp_pts[p].col] = current_cshape.ishape.name;
}