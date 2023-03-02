#include <curses.h>
#include <ncurses.h>

#include "context.h"
#include "logging.h"
#include "filesystem.h"
#include <stdlib.h>

void update_menu(Context *ctx) {
  debug_print("%s\n", "updating the menu");

  char* path = join_path(ctx->path_list);

  free(ctx->cur_fs_entities);
  size_t cur_fs_entities_sz;
  ctx->cur_fs_entities = get_filesystem_entities(path, &cur_fs_entities_sz);
  free(path);

  build_menu(ctx->menu_state, ctx->cur_fs_entities, cur_fs_entities_sz);
  post_menu(ctx->menu_state->menu);
  refresh();
}

void main_loop(Context *ctx) {
  update_menu(ctx);

  refresh();
  int c;
  while (true) {
    c = getch();
    switch (c) {
    case KEY_UP:
      break;
    case KEY_DOWN:
      return;
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
