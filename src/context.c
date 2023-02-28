#include "context.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include <ncurses.h>

#include "path_list.h"
#include "logging.h"

void init_context(Context *ctx) {
    debug_print("%s\n", "initializing the context");
    getmaxyx(stdscr, ctx->rows, ctx->cols);
    
    char* path = calloc(PATH_MAX + 1, sizeof(*path));
    getcwd(path, PATH_MAX + 1);
    if (errno != 0) {
        perror("getwd");
        debug_print("%s\n", "can't get path");
        exit(EXIT_FAILURE);
    }
    ctx->path_list = split_path(path);
    free(path);

    ctx->menu_state = NULL;
}

void destroy_context(Context *ctx) {
    debug_print("%s\n", "destroying the context");
    destroy_path_list(ctx->path_list);
    ctx->path_list = NULL;
    destroy_menu_state(ctx->menu_state);
    ctx->menu_state = NULL;
}