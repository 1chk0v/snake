/* snake-0.3 Oct 24 2019  -- 13 years later ;-), changed ACS_BULLET to ACS_CKBOARD */
/* snake-0.2 (03/01/2006) -- added borders, colors and score system */
/* snake-0.1 (02/01/2006) -- initial version                        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>

void init_curses();
void print_snake(int dir);
void print_apple();
void endgame();
int inbounds();
struct snake {
	int x;
	int y;
} SNAKE[2048];
int x, y, appx, appy;
int i, c, app, score, speed, lastc, snlen = 4;
WINDOW *scrwin, *snwin;

int main() {
	printf("Set your speed (1, 2, 3...): ");
	scanf("%d", &speed);
	init_curses();
	memset(SNAKE, 0, sizeof(SNAKE));
	move(y = LINES/2, x = COLS/2-4);
	for(i = 0; i < snlen; i++) {
		SNAKE[i].x = x--;
		SNAKE[i].y = y;
	}
	c = KEY_RIGHT;
	do {
		print_apple();
		print_snake(c);
		for(i = 1; i < snlen; i++) {
			if(SNAKE[0].x == SNAKE[i].x && SNAKE[0].y == SNAKE[i].y)
				endgame();
		}
		if(SNAKE[0].x == appx && SNAKE[0].y == appy) {
			app = 0;
			score += 10;
			snlen += 4;
			for(i = 0; i < 4; i++)
				SNAKE[snlen-1-i] = SNAKE[0];
		}		
		if ((c = getch()) == ERR)
			c = lastc;
	} while (inbounds());	
	endgame();
	return 0;
}

void init_curses() {
	initscr();
	start_color();
	scrwin = newwin(1, COLS, 0, 0);
	snwin = newwin(LINES-1, COLS, 1, 0);
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	noecho();
	bkgd(COLOR_PAIR(1));
	attron(A_BOLD);
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	curs_set(0);
	mvhline(1, 1, 0, COLS-2);
	mvhline(LINES-1, 1, 0, COLS-2);
	mvvline(2, 0, 0, LINES-3);
	mvvline(2, COLS-1, 0, LINES-3);
	mvaddch(1, 0, ACS_ULCORNER);
	mvaddch(1, COLS-1, ACS_URCORNER);
	mvaddch(LINES-1, 0, ACS_LLCORNER);
	mvaddch(LINES-1, COLS-1, ACS_LRCORNER);
}

int inbounds() {
	if(SNAKE[0].y > 1 && SNAKE[0].y < LINES-1 && SNAKE[0].x > 0 && SNAKE[0].x < COLS-1)
		return 1;
	else return 0;
}

void print_snake(int c) {
	mvwprintw(scrwin, 0, 0, "Score: %d", score);
	wrefresh(scrwin);
	for(i = snlen-2; i >= 0; i--) {
		SNAKE[i+1] = SNAKE[i];
	}
	switch(c) {
		case KEY_UP: 
			if(lastc == KEY_DOWN) {
				SNAKE[0].y++;
				c = lastc;
			}
			else
				SNAKE[0].y--;
			break;
		case KEY_DOWN: 
			if(lastc == KEY_UP) {
				SNAKE[0].y--;
				c = lastc;
			}
			else
				SNAKE[0].y++;
			break;
		case KEY_LEFT: 
			if(lastc == KEY_RIGHT) {
				SNAKE[0].x++;
				c = lastc;
			}
			else
				SNAKE[0].x--;
			break;
		case KEY_RIGHT: 
			if(lastc == KEY_LEFT) {
				SNAKE[0].x--;
				c = lastc;
			}
			else
				SNAKE[0].x++;
			break;
	}
	for(i = 0; i < snlen; i++) {
		move(SNAKE[i].y, SNAKE[i].x);
		if(i == 0)
			addch(ACS_CKBOARD | COLOR_PAIR(2));
		else
			addch(ACS_CKBOARD);
	}
	refresh();
	usleep(100000/speed);
	for(i = 0; i < snlen; i++) {
		move(SNAKE[i].y, SNAKE[i].x);
		addch(' ');
	}
	lastc = c;
}

void print_apple() {
	int curx, cury;
	srand(time(NULL));
	while(!app) {
		appx = rand() % COLS;
		appy = rand() % LINES;
		app = 1;
    		if(appx < 1 || appx > COLS-2 || appy < 2 || appy > LINES-2)
			app = 0;
		for(i = 0; i < snlen; i++) {
			if(appx == SNAKE[i].x && appy == SNAKE[i].y)
				app = 0;
		}
	}
	getyx(stdscr, cury, curx);
	move(appy, appx);
	addch(ACS_DIAMOND | COLOR_PAIR(3));
	refresh();
	move(cury, curx);
}

void endgame() {
	endwin();
	printf("Thanks for playing \"snake\"\nYour score is %d\n", score);
	exit(0);
}

