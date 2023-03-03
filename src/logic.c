#include "logic.h"

#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include "menu_state.h"

// returns true if ok, false otherwise
bool update_menu(Context *ctx) {
  debug_print("%s\n", "updating the menu");

  char *path = join_path(ctx->path_list);

  size_t new_fs_entities_sz;
  FilesystemEntity *new_fs_entities = get_filesystem_entities(
      path, &new_fs_entities_sz, ctx->show_hidden, ctx->cols);
  free(path);
  if (new_fs_entities == NULL) {
    debug_print("%s\n", "can't get fs_entities");
    return false;
  }
  destroy_fs_entities(ctx->cur_fs_entities, ctx->cur_fs_entities_sz);
  ctx->cur_fs_entities = new_fs_entities;
  ctx->cur_fs_entities_sz = new_fs_entities_sz;
  build_menu(ctx->menu_state, ctx->cur_fs_entities, ctx->cur_fs_entities_sz);
  post_menu(ctx->menu_state->menu);
  refresh();
  return true;
}

void walk(Context *ctx) {
  const FilesystemEntity *cur_fs_entity = get_cur_fs_entity(ctx->menu_state);
  if (cur_fs_entity->entity_type != ET_DIRECTORY) {
    return;
  }

  if (is_parent_directory(cur_fs_entity) && ctx->path_list->nodes_cnt > 0) {
    PathNode *node = pop_back_no_free(ctx->path_list);
    if (!update_menu(ctx)) {
      push_back_node(ctx->path_list, node);
    } else {
      free_node(node);
    }
  } else {
    char *val = calloc(1, strlen(cur_fs_entity->name) + 1);
    if (val == NULL) {
      debug_print("%s\n", "can't alloc");
      exit(EXIT_FAILURE);
    }
    strcpy(val, cur_fs_entity->name);
    push_back(ctx->path_list, val);
    if (!update_menu(ctx)) {
      pop_back(ctx->path_list);
    }
  }

  refresh();
}

void toggle_hidden(Context *ctx) {
  ctx->show_hidden ^= 1;
  update_menu(ctx);
}