#ifndef H_CONTEXT
#define H_CONTEXT

#include "menu_state.h"
#include "path_list.h"
#include "filesystem.h"

typedef struct Context {
  unsigned rows;
  unsigned cols;
  
  MenuState *menu_state;
  
  PathList *path_list;

  FilesystemEntity* cur_fs_entities;
  size_t cur_fs_entities_sz;
} Context;

void init_context(Context *ctx);
void destroy_context(Context *ctx);

#endif //  H_CONTEXT