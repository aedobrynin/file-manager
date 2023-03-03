#include "context.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "logging.h"
#include "path_list.h"

void init_context(Context *ctx) {
  debug_print("%s\n", "initializing the context");
  getmaxyx(stdscr, ctx->rows, ctx->cols);

  char *path = calloc(PATH_MAX + 1, sizeof(*path));
  getcwd(path, PATH_MAX + 1);
  if (errno != 0) {
    perror("getwd");
    debug_print("%s\n", "can't get path");
    exit(EXIT_FAILURE);
  }
  ctx->path_list = split_path(path);
  free(path);

  ctx->menu_state = calloc(1, sizeof(*ctx->menu_state));
  if (ctx->menu_state == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }
  init_menu_state(ctx->menu_state);

  ctx->cur_fs_entities = NULL;
  ctx->cur_fs_entities_sz = 0;

  ctx->show_hidden = false;

  ctx->copy_ctx.copy_from = NULL;
  ctx->copy_ctx.filename = NULL;
}

void free_copy_context(Context *ctx) {
  free(ctx->copy_ctx.copy_from);
  free(ctx->copy_ctx.filename);
}

void destroy_context(Context *ctx) {
  debug_print("%s\n", "destroying the context");
  destroy_path_list(ctx->path_list);
  ctx->path_list = NULL;
  destroy_menu_state(ctx->menu_state);
  ctx->menu_state = NULL;
  free_copy_context(ctx);
  destroy_fs_entities(ctx->cur_fs_entities, ctx->cur_fs_entities_sz);
}