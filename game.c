#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


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


	game_win = newwin(ROW_GRID+2, COL_GRID*2+2, 0, 0);
	box(game_win, 0, 0);
	wrefresh(game_win);
	wtimeout(game_win, 0);
	wbkgd(game_win, COLOR_PAIR(rand() % SIZE_SHAPE));
}

void update_screen() {
	display_grid();
	wrefresh(game_win);
}

void loop() {
	clock_t start = clock();
	float delay = DEFAULT_DELAY;
	int key, pre_key;
	int update_shape = TRUE;
	int check_remove_row = TRUE;
	current_cshape = insert_shape(pick_shape());

	do {
		if (update_shape) {
			next_shape = pick_shape();
			update_shape = FALSE;
			check_remove_row = TRUE;
		} else
			check_remove_row = FALSE;

		if (((float)(clock() - start) / CLOCKS_PER_SEC) > delay) {
			if (move_down() == TRUE) {
				current_cshape = insert_shape(next_shape);
				update_shape = TRUE;
				continue;
			};	
			start = clock();
		}
		if (check_remove_row && remove_filled_rows()) {
			check_remove_row = FALSE;
			usleep(100000);
		}
		key = wgetch(game_win);
		if (pre_key == KEY_DOWN || pre_key == 'W' || pre_key == 'w' &&
			pre_key != KEY_DOWN || pre_key != 'W' || pre_key != 'w')
			delay = DEFAULT_DELAY;
		switch (key) {
		case 'w': case 'W': case KEY_UP:
			shape_rotate_right();
			break;
		case 's': case 'S': case KEY_DOWN:
			delay = SPEEDUP_DELAY;
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
		pre_key = key;
		update_screen();
	} while (!losed);
	endwin();
}