#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "tetris.h"
#include "game.h"

extern struct shape next_shape;
extern struct c_shape current_cshape;
extern int level;
extern int rgb;
extern int color;

void init_windows() {
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

	int random_color = color == -1 ? rand() % SIZE_SHAPE : color;
	game_win = newwin(ROW_GRID+2, COL_GRID*2+2, 0, 0);
	box(game_win, 0, 0);
	wattron(game_win, A_BOLD);
	mvwprintw(game_win, 0, 1, "Board");
	wattroff(game_win, A_BOLD);
	wtimeout(game_win, 100);
	wbkgd(game_win, COLOR_PAIR(random_color));
	wrefresh(game_win);

	score_win = newwin(SCORE_HEIGHT, 15, 0, COL_GRID*2+3);
	box(score_win, 0, 0);
	wattron(score_win, A_BOLD);
	mvwprintw(score_win, 0, 1, "Info");
	wattroff(score_win, A_BOLD);
	wbkgd(score_win, COLOR_PAIR(random_color));
	wrefresh(score_win);

	preview_shape_win = newwin(PREVIEW_HEIGHT, 15, SCORE_HEIGHT, COL_GRID*2+3);
	box(preview_shape_win, 0, 0);
	wattron(preview_shape_win, A_BOLD);
	mvwprintw(preview_shape_win, 0, 1, "Next");
	wattroff(preview_shape_win, A_BOLD);
	wbkgd(preview_shape_win, COLOR_PAIR(random_color));
	wrefresh(preview_shape_win);

	hint_win = newwin(8, 35, ROW_GRID-6, COL_GRID*2+3);
	box(hint_win, 0, 0);
	wattron(hint_win, A_BOLD);
	mvwprintw(hint_win, 0, 1, "Hints");
	wattroff(hint_win, A_BOLD);
	wbkgd(hint_win, COLOR_PAIR(random_color));
	display_hints();
	wrefresh(hint_win);
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
	mvwprintw(score_win, 1, 1, "Score: %d", score);
	mvwprintw(score_win, 2, 1, "Level: %d", level);
}

void update_screen() {
	static int color_id = 0;
	static int change_color_delay = 0;
	change_color_delay -= 100;
	if (rgb == TRUE && change_color_delay <= 0) {
		change_color_delay = 800 * pow(0.9, 20);
		wbkgd(game_win, COLOR_PAIR(color_id%SIZE_SHAPE));
		wbkgd(score_win, COLOR_PAIR(color_id%SIZE_SHAPE));
		wbkgd(preview_shape_win, COLOR_PAIR(color_id%SIZE_SHAPE));
		wbkgd(hint_win, COLOR_PAIR(color_id%SIZE_SHAPE));
		color_id++;
		wrefresh(hint_win);
	}
	display_grid();
	preview_shape();
	show_score();
	wrefresh(game_win);
	wrefresh(score_win);
	wrefresh(preview_shape_win);
}

void write_text(WINDOW* win, char* text) {
	wattron(hint_win, A_BOLD);
	static int i = 0;
	i++;
	int j = 1;
	for (char* c = text; *c != '\0' && *c != '\n'; c++) {
		mvwaddch(win, i, j++, *c);
		if (*c == ':')
			wattroff(hint_win, A_BOLD);
	}
}

void display_hints() {
	write_text(hint_win, "left/right (a/d): move left/right");
	write_text(hint_win, "up (w): rotate shape");
	write_text(hint_win, "down (s): move down faster");
	write_text(hint_win, "space (0): drop shape");
	write_text(hint_win, "p/P: pause or resume");
	write_text(hint_win, "Q/q: quite game");
}

void destroy_game() {
	delwin(game_win);
	delwin(score_win);
	delwin(preview_shape_win);
	clear();
	endwin();
	exit(0);
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
			delay = 800 * pow(0.9, level);
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
		case 'p': case 'P':
			pause_game();
			break;
		case 'q': case 'Q':
			destroy_game();
			break;
		}
		update_screen();
	} while (!losed);
	destroy_game();
}

void pause_game() {
	int key;
	int x, y;
	wattron(game_win, A_BOLD);
	mvwprintw(game_win, 0, 1, "Paused");
	while (1) {
		key = wgetch(game_win);
		if (key == 'p') break;
		else if (key == 'q' || key == 'Q') destroy_game();
	}
	for (int i = 3; i > 0; i--) {
		mvwprintw(game_win, 0, 1, "Play in %d", i);
		wrefresh(game_win);
		usleep(1000000);
	}
	box(game_win, 0, 0);
	wattroff(game_win, A_BOLD);
	mvwprintw(game_win, 0, 1, "Board");
}