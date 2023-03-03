#pragma once

#include "filesystem.h"
#include "menu_state.h"
#include "path_list.h"

typedef struct CopyContext {
  char *copy_from;
  char *filename;
  bool is_cut; // if true, deletes file in end_copy().
} CopyContext;

typedef struct Context {
  unsigned rows;
  unsigned cols;

  MenuState *menu_state;

  PathList *path_list;

  FilesystemEntity *cur_fs_entities;
  size_t cur_fs_entities_sz;

  bool show_hidden;

  CopyContext copy_ctx;
} Context;

void init_context(Context *ctx);
void free_copy_context(Context *ctx);
void destroy_context(Context *ctx);
