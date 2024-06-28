#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#include "tetris.h"
#include "game.h"

struct shape SHAPES[SIZE_SHAPE] = {
	{
		.name='O',
		.color=COLOR_YELLOW,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','O','O','\0'},
			{'\0','O','O','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=1
	},
	{
		.name='L',
		.color=COLOR_ORANGE,
		.structure=
			{{'\0','L','\0','\0'},
			{'\0','L','\0','\0'},
			{'\0','L','L','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=1
	},
	{
		.name='J',
		.color=COLOR_BLUE,
		.structure=
			{{'\0','J','\0','\0'},
			{'\0','J','\0','\0'},
			{'J','J','\0','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	},
	{
		.name='I',
		.color=COLOR_CYAN,
		.structure=
			{{'\0','\0','I','\0'},
			{'\0','\0','I','\0'},
			{'\0','\0','I','\0'},
			{'\0','\0','I','\0'}},
		.init_position=2
	},
	{
		.name='S',
		.color=COLOR_GREEN,
		.structure=
			{{'\0','\0','\0','\0'},
			{'\0','S','S','\0'},
			{'S','S','\0','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	},
	{
		.name='Z',
		.color=COLOR_RED,
		.structure=
			{{'\0','\0','\0','\0'},
			{'Z','Z','\0','\0'},
			{'\0','Z','Z', '\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	},
	{
		.name='T',
		.color=COLOR_MAGENTA,
		.structure=
			{{'\0','\0','\0','\0'},
			{'T','T','T','\0'},
			{'\0','T','\0','\0'},
			{'\0','\0','\0','\0'}},
		.init_position=0
	}
};
int ORIGIN_RULES[SIZE_SHAPE][4][2] = {
	{{-1, -1}, 	{-1, -1}, 	{-1, -1}, 	{-1, -1}}, 		// O_SHAPE
	{{0, -1}, 	{-1, -1}, 	{-1, -1}, 	{-1, -2}}, 		// L_SHAPE
	{{0, -1}, 	{0, -1}, 	{-1, -2}, 	{-2, 0}}, 		// J_SHAPE
	{{0, -2}, 	{-2, 0}, 	{0, -1}, 	{-1, 0}}, 		// I_SHAPE
	{{-1, -1}, 	{0, -1}, 	{-1, -2}, 	{-1, -1}}, 		// S_SHAPE
	{{-1, 0}, 	{0, -2}, 	{-1, -1}, 	{-1, -2}}, 		// Z_SHAPE
	{{-1, 0}, 	{0, -2}, 	{-1, -2}, 	{-1, -1}}, 		// T_SHAPE
};
struct shape next_shape;
char board[ROW_GRID][COL_GRID] = {};
struct c_shape current_cshape;
int losed = FALSE;
int score = 0;
int level = 1;
int color = -1; // randomly
int rgb = FALSE;
WINDOW* game_win, *preview_shape_win, *score_win, *hint_win, *prompt_win;

char HELP_TEXT[] = "Usage: ./tetris [options...]\n"
				   " -h, --help		show this help text.\n"
				   " -l, --level		set game level, between [1-6], default 1.\n"
				   " -b, --background	set background color of game, default \"randomly\".\n"
				   " -R, --rgb		apply rgb effect to game.\n\n"
				   "list of avaiable colors:\n"
				   " - yellow\n - orange\n - blue\n - cyan\n - green\n - red\n - magenta\n\n";

int main(int argc, char** argv) {
	for (argc--,argv++; argc > 0; argc--,argv++) {
		if (strcmp(*argv, "--help") == 0 || strcmp(*argv, "-h") == 0) {
			printf("%s", HELP_TEXT);
			exit(1);
		} else if (strcmp(*argv, "--rgb") == 0 || strcmp(*argv, "-R") == 0) {
			rgb = TRUE;
		} else if (strcmp(*argv, "--level") == 0 || strcmp(*argv, "-l") == 0) {
			if (*(argv+1) != NULL && atoi(*(argv+1)) > 0 && atoi(*(argv+1)) < 7) {
				level = atoi(*(argv+1));
				argv++; argc--;
			} else {
				printf("[-] ERROR: level between [1-6] must be provide!\n");
				exit(1);
			}
		} else if (strcmp(*argv, "-b") == 0 || strcmp(*argv, "--background") == 0) {
			if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "YELLOW") == 0))
				color = 1;
			else if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "ORANGE") == 0))
				color = 2;
			else if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "BLUE") == 0))
				color = 3;
			else if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "CYAN") == 0))
				color = 4;
			else if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "GREEN") == 0))
				color = 5;
			else if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "RED") == 0))
				color = 6;
			else if (*(argv+1) != NULL && (strcasecmp(*(argv+1), "MAGENTA") == 0))
				color = 7;
			else {
				printf("[-] ERROR: invalid '%s' color given!, -h, --help: more info\n", *(argv+1));
				exit(1);
			}
			argv++; argc--;
		} else {
			printf("[-] ERROR: invalid options '%s'\n", *argv);
			exit(1);
		}
	}
	srand(time(NULL));
	setlocale(LC_ALL, "");

	init_windows();
	loop();
}