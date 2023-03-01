#include <curses.h>
#include <ncurses.h>

#include "context.h"
#include "logging.h"


void main_loop(Context *ctx) {
  printw("Hello World !!!");
  refresh();
  int c;
  while (1) {
    c = getch();
    switch (c) {
    case KEY_UP:
      break;
    case KEY_DOWN:
      break;
    case KEY_LEFT:
      break;
    default:
      break;
    }

    refresh();
  }
}

int main() {
  init_logging();
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  clear();

  Context ctx;
  init_context(&ctx);

  main_loop(&ctx);

  destroy_context(&ctx);
  endwin();

  return 0;
}
