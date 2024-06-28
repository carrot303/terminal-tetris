#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "tetris.h"
#include "game.h"

extern struct shape next_shape;
extern struct c_shape current_cshape;
extern int level;
extern int rgb;
extern int color;

int restarts = 0;
int pre_level;

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

	score_win = newwin(SCORE_HEIGHT, 14, 0, COL_GRID*2+3);
	box(score_win, 0, 0);
	wattron(score_win, A_BOLD);
	mvwprintw(score_win, 0, 1, "Info");
	wattroff(score_win, A_BOLD);
	wbkgd(score_win, COLOR_PAIR(random_color));
	wrefresh(score_win);

	preview_shape_win = newwin(PREVIEW_HEIGHT, 14, SCORE_HEIGHT, COL_GRID*2+3);
	box(preview_shape_win, 0, 0);
	wattron(preview_shape_win, A_BOLD);
	mvwprintw(preview_shape_win, 0, 1, "Next");
	wattroff(preview_shape_win, A_BOLD);
	wbkgd(preview_shape_win, COLOR_PAIR(random_color));
	wrefresh(preview_shape_win);

	hint_win = newwin(9, 35, ROW_GRID-7, COL_GRID*2+3);
	box(hint_win, 0, 0);
	wattron(hint_win, A_BOLD);
	mvwprintw(hint_win, 0, 1, "Hotkeys");
	wattroff(hint_win, A_BOLD);
	wbkgd(hint_win, COLOR_PAIR(random_color));
	display_hints();
	wrefresh(hint_win);

	prompt_win = newwin(4, COL_GRID*4, ROW_GRID+2, 0);
	wbkgd(prompt_win, COLOR_PAIR(random_color));
	wattron(prompt_win, A_BOLD);
	wrefresh(prompt_win);

}

void preview_shape() {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (next_shape.structure[r][c]) {
				wattron(preview_shape_win, COLOR_PAIR(SHAPE_INDEX(next_shape.name)+1));
				mvwprintw(preview_shape_win, r+2, c*2+2, BLOCK);
				wattroff(preview_shape_win, COLOR_PAIR(SHAPE_INDEX(next_shape.name)+1));
			} else {
				mvwprintw(preview_shape_win, r+2, c*2+2, "  ");
			}
		}
	}
}

void show_score() {
	mvwprintw(score_win, 1, 1, "Score: %d", score);
	mvwprintw(score_win, 2, 1, "Level: %d", level);
	mvwprintw(score_win, 3, 1, "Attempts: %d", restarts);
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

void write_text(WINDOW* win, char* text, int i) {
	wattron(hint_win, A_BOLD);
	int j = 1;
	for (char* c = text; *c != '\0' && *c != '\n'; c++) {
		mvwaddch(win, i, j++, *c);
		if (*c == ':')
			wattroff(hint_win, A_BOLD);
	}
}

void display_hints() {
	write_text(hint_win, "left/right (a/d): move left/right", 1);
	write_text(hint_win, "up (w): rotate shape", 2);
	write_text(hint_win, "down (s): move down faster", 3);
	write_text(hint_win, "space (0): drop shape", 4);
	write_text(hint_win, "p/P: pause or resume", 5);
	write_text(hint_win, "r/R: restart game", 6);
	write_text(hint_win, "Q/q: quite game", 7);
}

int destroy_game() {
	int key; 
	if (losed) {
		while (1) {
			key = prompt_user("Sorry you losed :) restart game? [Y/n]");
			if (key == '\n' || key == 'y' || key == 'Y' || key == 'n' || key == 'N') {
				break;
			}
		}
		if (key == 'y' || key == 'Y' || key == '\n') {
			reset_game();
			return TRUE;
		}
	} else {
		while (1) {
			key = prompt_user("Are you sure to exit? [Y/n]");
			if (key == '\n' || key == 'y' || key == 'Y' || key == 'n' || key == 'N') {
				break;
			}
		}
		if (key == 'n' || key == 'N')
			return FALSE;
	}
	delwin(prompt_win);
	delwin(game_win);
	delwin(score_win);
	delwin(preview_shape_win);
	delwin(hint_win);
	clear();
	endwin();
	exit(0);
	return FALSE;
}

void loop() {
	int key;
	int update_shape = FALSE;
	int delay = 0;
	int level_up_delay = 0;
	int i = 1;
	int level_up = FALSE;
	pre_level = level;
	restarts++;
	current_cshape = insert_shape(pick_shape());
	next_shape = pick_shape();
	if (restarts > 5) {
		endwin();
		printf("[-] Sorry, you cannot restart anymore :/\n");
		exit(0);
	}
	do {
		if (score >= (LEVEL_UP_POINT*i) && level < 20) {
			i++;
			level++;
			write_text(prompt_win, "LEVEL UP", 0);
			wrefresh(prompt_win);
			level_up = TRUE;
		} 
		update_screen();
		if (update_shape) {
			if (remove_filled_rows())
				usleep(100000);
			current_cshape = insert_shape(next_shape);
			next_shape = pick_shape();
			update_shape = FALSE;
			continue;
		}
		level_up_delay -= 100;
		if (level_up_delay <= 0) {
			level_up_delay = 800 * pow(0.9, 1);
			if (level_up) {
				level_up = FALSE;
				wclear(prompt_win);
				wrefresh(prompt_win);	
			}
		}
		delay -= 100;
		if (delay <= 0) {
			delay = 800 * pow(0.9, level);
			if (move_down() == TRUE) {
				update_shape = TRUE;
				continue;
			};
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
			update_shape = TRUE;
			break;
		case 'p': case 'P':
			pause_game();
			break;
		case 'r': case 'R':
			key = prompt_user("Are you sure to reset? [Y/n]");
			if (key == 'y' || key == 'Y' || key == '\n') {
				reset_game();
				return loop();
			}
			break;
		case 'q': case 'Q':
			destroy_game();
			break;
		}
	} while (!losed);
	if (destroy_game() == TRUE)
		return loop();
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

int prompt_user(char* prompt) {
	int ch;
	mvwprintw(prompt_win, 0, 1, "%s", prompt);
	while ((ch = wgetch(prompt_win)) == ERR)
		;
	wclear(prompt_win);
	wrefresh(prompt_win);
	return ch;
}

void reset_game() {
	memset(board, '\0', ROW_GRID*COL_GRID);
	score = 0;
	losed = FALSE;
	level = pre_level;
	wclear(score_win);
	box(score_win, 0, 0);
	wattron(score_win, A_BOLD);
	mvwprintw(score_win, 0, 1, "Info");
	wattroff(score_win, A_BOLD);
}