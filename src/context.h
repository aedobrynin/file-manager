#include <menu.h>

typedef struct MenuState {
    MENU* menu;
    size_t items_cnt;
    ITEM** items;
} MenuState;



typedef struct Context {
    unsigned rows;
    unsigned cols;
    char* cur_path;
    MenuState cur_menu_state;
    PathNode* path_head;
} Context;