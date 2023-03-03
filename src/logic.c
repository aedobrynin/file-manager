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
    char *val = strdup(cur_fs_entity->name);
    if (val == NULL) {
      debug_print("%s\n", "can't strdup");
      perror("strdup");
      exit(EXIT_FAILURE);
    }
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

void delete_file(Context *ctx) {
  const FilesystemEntity *cur_fs_entity = get_cur_fs_entity(ctx->menu_state);
  char *name_dup = strdup(cur_fs_entity->name);
  if (name_dup == NULL) {
    debug_print("%s\n", "can't alloc");
    return;
  }
  push_back(ctx->path_list, name_dup);
  char *path = join_path(ctx->path_list);
  debug_print("deleting at path %s\n", path);
  recursive_delete(path, cur_fs_entity->entity_type);
  pop_back(ctx->path_list);
  free(path);
  update_menu(ctx);
}

void start_copy(Context *ctx) {
  free_copy_context(ctx);

  const FilesystemEntity *cur_fs_entity = get_cur_fs_entity(ctx->menu_state);
  if (cur_fs_entity->entity_type == ET_DIRECTORY) {
    debug_print("%s\n", "can't copy a directory");
    return;
  }
  char *name_dup = strdup(cur_fs_entity->name);
  if (name_dup == NULL) {
    debug_print("%s\n", "can't alloc");
    return;
  }
  push_back(ctx->path_list, name_dup);
  ctx->copy_ctx.copy_from = join_path(ctx->path_list);
  pop_back(ctx->path_list);
  ctx->copy_ctx.filename = strdup(cur_fs_entity->name);
  if (ctx->copy_ctx.filename == NULL) {
    debug_print("%s\n", "can't alloc");
    return;
  }
}

void end_copy(Context *ctx) {
  if (ctx->copy_ctx.copy_from == NULL) {
    debug_print("%s\n", "can't copy, copy_from is not set");
    return;
  }

  push_back(ctx->path_list, ctx->copy_ctx.filename);
  char *copy_to = join_path(ctx->path_list);
  pop_back(ctx->path_list);

  char *copy_from = ctx->copy_ctx.copy_from;
  ctx->copy_ctx.copy_from = NULL;
  ctx->copy_ctx.filename = NULL;

  if (strcmp(copy_from, copy_to) == 0) {
    debug_print("%s\n", "can't copy to the same path");
    free(copy_from);
    free(copy_to);
    return;
  }
  debug_print("trying to copy from %s to %s\n", copy_from, copy_to);
  int res = copy_file(copy_from, copy_to);
  free(copy_from);
  free(copy_to);
  if (res == 0) {
    update_menu(ctx);
  }
}