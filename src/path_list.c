#include "path_list.h"


static size_t result_path_len(const PathList* list) {
    // TODO
}

char* join_path(const PathList* list) {
    size_t buf_len = result_path_len(list);
    char* buf = calloc(buf_len + 1, sizeof(*buf));
    // TODO
    return buf;
}


PathList* split_path(const char* path) {
    // TODO
}

void pop_back(PathList* list) {
    if (list == NULL || list->last == NULL) {
        exit(EXIT_FAILURE);
    }

    PathNode* to_free = list->last;
    list->last = list->last->prev;
    if (list->last) {
        list->last->next = NULL;
    } else {
        list->first = NULL;
    }
    free(to_free);
}

void push_back(PathList* list, const char* value) {
    if (list == NULL) {
        exit(EXIT_FAILURE);
    }

    PathNode* to_add = calloc(1, sizeof(*to_add));
    to_add->value = value;
    to_add->next = NULL;
    to_add->prev = list->last;
    if (list_last) {
        list->last->next = to_add;
        list->last = to_add;
    } else {
        list->first = to_add;
        list->last = to_add;
    }
}

void free_path_list(PathList* list) {
    if (list == NULL) {
        return;
    }

    PathNode* cur = list->first;
    while (cur) {
        PathNode* nxt = cur->next;
        free(cur);
        cur = nxt;
    }
}