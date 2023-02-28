#include "context.h"
#include <menu.h>
#include <ncurses.h>

#include "logging.h"

int main() {
  init_logging();

  initscr();
  clear();
  Context ctx;
  getmaxyx(stdscr, ctx.rows, ctx.cols);

  printw("Hello World !!!");

  refresh();
  debug_print("%s\n", "kek");
  getch();
  endwin();
  return 0;
}
