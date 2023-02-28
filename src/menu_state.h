#ifndef H_MENU_STATE
#define H_MENU_STATE

#include <menu.h>

typedef struct MenuState {
  MENU *menu;
  size_t items_cnt;
  ITEM **items;
} MenuState;

void destroy_menu_state(MenuState *state);

#endif //  H_MENU_STATE