#include <curses.h>
#include <ncurses.h>

#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "filesystem.h"
#include "logging.h"

void update_menu(Context *ctx) {
  debug_print("%s\n", "updating the menu");

  char *path = join_path(ctx->path_list);

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
    case KEY_LEFT:
      return;
      break;
    case REAL_ENTER: {
      const FilesystemEntity *cur_fs_entity =
          get_cur_fs_entity(ctx->menu_state);
      if (cur_fs_entity->entity_type != ET_DIRECTORY) {
        continue;
      }

      if (is_parent_directory(cur_fs_entity) && ctx->path_list->nodes_cnt > 0) {
        pop_back(ctx->path_list);
      } else {
        char *val = calloc(1, strlen(cur_fs_entity->name) + 1);
        if (val == NULL) {
          debug_print("%s\n", "can't alloc");
          exit(EXIT_FAILURE);
        }
        strcpy(val, cur_fs_entity->name);
        push_back(ctx->path_list, val);
      }
      update_menu(ctx);
    }
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
