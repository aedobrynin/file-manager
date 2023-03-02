#ifndef H_MENU_STATE
#define H_MENU_STATE

#include <menu.h>

#include "filesystem.h"

typedef struct MenuState {
  MENU *menu;
  size_t items_cnt;
  ITEM **items;
} MenuState;

void init_menu_state(MenuState *state);

void build_menu(MenuState* state, FilesystemEntity* fs_entites, size_t fs_ent_sz);

void destroy_menu_state(MenuState *state);


#endif //  H_MENU_STATE