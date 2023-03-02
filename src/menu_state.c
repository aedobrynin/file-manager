#include "menu_state.h"

#include <stdlib.h>

#include <menu.h>
#include <ncurses.h>
#include <curses.h>

static void destroy_internal_menu(MenuState *state) {
  if (state == NULL) {
    debug_print("%s\n", "Warning! State is null!");
    return;
  }

  if (state->menu == NULL) {
    return;
  }

  free_menu(state->menu);  
  for(size_t i = 0; i < state->items_cnt; ++i) {
    free_item(state->items[i]);
  }
  free(state->items);
}

void init_menu_state(MenuState* state) {
  state->menu = NULL;
  state->items = NULL;
  state->items_cnt = 0;
}

void build_menu(MenuState* state, FilesystemEntity* fs_entities, size_t fs_ent_sz) {
  unpost_menu(state->menu);
  destroy_internal_menu(state);
  state->items = calloc(fs_ent_sz + 1, sizeof(*state->items));
  if (state->items == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < fs_ent_sz; ++i) {
    state->items[i] = new_item(fs_entities[i].name, fs_entities[i].name);
    set_item_userptr(state->items[i], (void*)(fs_entities + i));
  }
  state->items[fs_ent_sz] = NULL;
  state->items_cnt = fs_ent_sz;

  state->menu = new_menu(state->items);
  menu_opts_off(state->menu, O_SHOWDESC);
}

void menu_up(MenuState* state) {
  menu_driver(state->menu, REQ_UP_ITEM);
}

void menu_down(MenuState* state) {
  menu_driver(state->menu, REQ_DOWN_ITEM);
}

void destroy_menu_state(MenuState *state) {
  if (state == NULL) {
    debug_print("%s\n", "Warning! State is null!");
    return;
  }
  unpost_menu(state->menu);
  destroy_internal_menu(state);
  free(state);
}