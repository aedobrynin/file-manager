#pragma once

#include <menu.h>

#include "filesystem.h"

typedef struct MenuState {
  MENU *menu;
  size_t items_cnt;
  ITEM **items;
} MenuState;

void init_menu_state(MenuState *state);

void build_menu(MenuState *state, FilesystemEntity *fs_entites,
                size_t fs_ent_sz);

void menu_up(MenuState *state);
void menu_down(MenuState *state);

const FilesystemEntity *get_cur_fs_entity(MenuState *state);

void destroy_menu_state(MenuState *state);
