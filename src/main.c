#include <ncurses.h>
#include <menu.h>
#include "context.h"

int main()
{
	initscr();
	clear();
	Context ctx;
	getmaxyx(stdscr, ctx.rows, ctx.cols);
	
	printw("Hello World !!!");
	refresh();

	getch();
	endwin();
	return 0;
}
