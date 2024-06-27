#include <stdlib.h>
#include <ncurses.h>

#include "tetris.h"
#include "game.h"


void display_cell(int r, int c) {
	if (board[r][c]) {
		wattron(game_win, COLOR_PAIR(SHAPE_INDEX(board[r][c])+1));
		mvwprintw(game_win, r+1, c*2+1, BLOCK);
		wattroff(game_win, COLOR_PAIR(SHAPE_INDEX(board[r][c])+1));
	}
	else
		mvwprintw(game_win, r+1, c*2+1, "  ");
}

void display_grid() {
	for (int r = 0; r < ROW_GRID; r++) {
		for (int c = 0; c < COL_GRID; c++) {
			display_cell(r, c);
		}
	}
}

struct shape pick_shape(void) {
	return SHAPES[rand() % SIZE_SHAPE];
}

struct c_shape insert_shape(struct shape ishape) {
	int cnt = 0;
	struct c_shape ic_shape;
	struct point p;
	ic_shape.ishape = ishape;
	ic_shape.direction = 0;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (ishape.structure[r][c] != '\0') {
				if (board[r][c+MIDDLE_COL-ishape.init_position] != '\0')
					losed = 1; 
				board[r][c+MIDDLE_COL-ishape.init_position] = ishape.structure[r][c];
				p.row = r;
				p.col = c+MIDDLE_COL-ishape.init_position;
				ic_shape.points[cnt++] = p;
			}
		}
	}
	return ic_shape;
}

int remove_filled_rows() {
	static int score_per_row[5] = {0, 40, 100, 300, 1200};
	int removed_count = 0;
	int r, c;
	for (r = 0; r < ROW_GRID; r++) {
		int filled = 1;
		for (c = 0; c < COL_GRID; c++)
			filled &= board[r][c] != '\0';
		if (filled) {
			removed_count++;
			for (int x = r; x > 4; x--) {
				for (int y = 0; y < COL_GRID; y++) {
					board[x][y] = board[x-1][y];
				}
			}
		}
	}
	score += score_per_row[removed_count];
	return removed_count > 0 ? TRUE : FALSE;
}