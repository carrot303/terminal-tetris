#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


#include "tetris.h"
#include "game.h"


extern struct shape next_shape;
extern struct c_shape current_cshape;

void init_windows(void) {
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);

	if (has_colors() != 1) {
		endwin();
		printf("Your terminal does not support color\n");
        exit(1);
	}
	
	
	start_color();
	for (int i = 1; i < 8; i++) init_pair(i, SHAPES[i-1].color, 0);

	int random_color = rand() % SIZE_SHAPE;
	game_win = newwin(ROW_GRID+2, COL_GRID*2+2, 0, 0);
	box(game_win, 0, 0);
	mvwprintw(game_win, 0, 1, "Board");
	wtimeout(game_win, 100);
	wbkgd(game_win, COLOR_PAIR(random_color));
	wrefresh(game_win);

	score_win = newwin(SCORE_HEIGHT,12, 0, COL_GRID*2+2);
	box(score_win, 0, 0);
	mvwprintw(score_win, 0, 1, "Score");
	wbkgd(score_win, COLOR_PAIR(random_color));
	wrefresh(score_win);

	preview_shape_win = newwin(6,12, SCORE_HEIGHT, COL_GRID*2+2);
	box(preview_shape_win, 0, 0);
	mvwprintw(preview_shape_win, 0, 1, "Next");
	wbkgd(preview_shape_win, COLOR_PAIR(random_color));
	wrefresh(preview_shape_win);
}

void preview_shape() {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (next_shape.structure[r][c]) {
				wattron(preview_shape_win, COLOR_PAIR(SHAPE_INDEX(next_shape.name)+1));
				mvwprintw(preview_shape_win, r+1, c*2+2, BLOCK);
				wattroff(preview_shape_win, COLOR_PAIR(SHAPE_INDEX(next_shape.name)+1));
			} else {
				mvwprintw(preview_shape_win, r+1, c*2+2, "  ");
			}
		}
	}
}

void show_score() {
	mvwprintw(score_win,1,1, "%d", score);
}

void update_screen() {
	display_grid();
	preview_shape();
	show_score();
	wrefresh(game_win);
	wrefresh(score_win);
	wrefresh(preview_shape_win);
}

void loop() {
	int key;
	int update_shape = TRUE;
	int check_remove_row = TRUE;
	int delay = 0;
	current_cshape = insert_shape(pick_shape());

	do {
		check_remove_row = update_shape;
		if (update_shape) {
			next_shape = pick_shape();
			update_shape = FALSE;
		}

		delay -= 100;
		if (delay <= 0) {
			delay = 800 * pow(0.9, 1);
			if (move_down() == TRUE) {
				current_cshape = insert_shape(next_shape);
				update_shape = TRUE;
				continue;
			};
		}
		if (check_remove_row && remove_filled_rows()) {
			check_remove_row = FALSE;
			usleep(100000);
		}
		key = wgetch(game_win);
		switch (key) {
		case 'w': case 'W': case KEY_UP:
			shape_rotate_right();
			break;
		case 's': case 'S': case KEY_DOWN:
			delay = 0;
			break;
		case 'd': case 'D': case KEY_RIGHT:
			move_right();
			break;
		case 'a': case 'A': case KEY_LEFT:
			move_left();
			break;
		case ' ': case '0': case '\n':
			drop_shape();
			current_cshape = insert_shape(next_shape);
			update_shape = TRUE;
			check_remove_row = TRUE;
			break;
		case 'q': case 'Q':
			exit(0);
			break;
		}
		update_screen();
	} while (!losed);
	endwin();
}