#include <ncurses.h>
#include <curses.h>

#include "context.h"
#include "logging.h"

int main() {
  init_logging();

  initscr();
  // noecho();
	// cbreak();	/* Line buffering disabled. pass on everything */
  // keypad(stdscr, TRUE);
  clear();

  Context ctx;
  init_context(&ctx);

  printw("Hello World !!!");
  refresh();

  // int c;
  // while(1)
	// {	c = getch();
	// 	switch(c)
	// 	{	case KEY_UP:
	// 			return 1;
	// 			break;
  //     default:
  //       break;
	// 	}
	// }	

  destroy_context(&ctx);
  endwin();

  return 0;
}
