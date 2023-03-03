#include <curses.h>
#include <ncurses.h>

#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "filesystem.h"
#include "logging.h"
#include "logic.h"

void main_loop(Context *ctx) {
  update_menu(ctx);

  refresh();
  int c;

  enum { REAL_ENTER = 10 };

  while (true) {
    c = getch();
    switch (c) {
    case KEY_UP:
      menu_up(ctx->menu_state);
      break;
    case KEY_DOWN:
      menu_down(ctx->menu_state);
      break;
    case REAL_ENTER:
      walk(ctx);
      break;
    case KEY_BACKSPACE:
      return;
    case 'H':
    case 'h':
      toggle_hidden(ctx);
      break;
    case 'D':
    case 'd':
      delete_file(ctx);
      break;
    case 'C':
    case 'c':
      start_copy(ctx);
      break;
    case 'V':
    case 'v':
      end_copy(ctx);
      break;
    default:
      break;
    }
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
