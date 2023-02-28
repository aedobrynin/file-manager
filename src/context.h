#ifndef H_CONTEXT
#define H_CONTEXT

#include "path_list.h"
#include "menu_state.h"

typedef struct Context {
  unsigned rows;
  unsigned cols;
  MenuState *menu_state;
  PathList *path_list;
} Context;

void init_context(Context *ctx);
void destroy_context(Context *ctx);

#endif //  H_CONTEXT