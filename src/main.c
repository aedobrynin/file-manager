#include "context.h"
#include <menu.h>
#include <ncurses.h>

int main() {
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
