#include "path_list.h"

#include <stdlib.h>
#include <string.h>

#include "logging.h"

char *join_path(const PathList *list) {
  if (list == NULL) {
    debug_print("%s\n", "list is NULL");
    exit(EXIT_FAILURE);
  }

  size_t buf_len = list->path_len + 1;
  char *buf = calloc(buf_len, sizeof(*buf));
  char *cur_buf_place = buf;
  PathNode *cur_node = list->first;
  while (cur_node) {
    *cur_buf_place = '/';
    ++cur_buf_place;
    strncpy(cur_buf_place, cur_node->val, cur_node->val_len);
    cur_buf_place += cur_node->val_len;
    cur_node = cur_node->next;
  }
  if (list->nodes_cnt == 0) {
    *cur_buf_place = '/';
    ++cur_buf_place;
  }
  *cur_buf_place = '\0';
  return buf;
}

PathList *split_path(const char *path) {
  if (path == NULL) {
    debug_print("%s\n", "path is NULL");
    exit(EXIT_FAILURE);
  }
  if (*path != '/') {
    debug_print("%s\n", "path should start with '/'");
    exit(EXIT_FAILURE);
  }
  ++path;

  PathList *list = calloc(1, sizeof(*list));

  char *slash_pos = strchr(path, '/');
  while (slash_pos != NULL) {
    size_t buf_size = slash_pos - path + 1;
    char *value = calloc(buf_size, sizeof(*value));
    strncpy(value, path, buf_size - 1);
    *(value + buf_size - 1) = '\0';
    push_back(list, value);
    path = slash_pos + 1;
    slash_pos = strchr(path, '/');
  }
  // TODO: append last path element
  return list;
}

void pop_back(PathList *list) {
  if (list == NULL || list->last == NULL) {
    debug_print("%s\n", "list is NULL or empty");
    exit(EXIT_FAILURE);
  }

  PathNode *to_free = list->last;
  list->last = list->last->prev;
  if (list->last) {
    list->last->next = NULL;
  } else {
    list->first = NULL;
  }

  list->path_len -= to_free->val_len;
  if (list->path_len != 1) {
    --list->path_len; // Remove slash
  }

  free(to_free);
  --list->nodes_cnt;
}

void push_back(PathList *list, char *val) {
  if (list == NULL) {
    debug_print("%s\n", "list is NULL");
    exit(EXIT_FAILURE);
  }

  PathNode *to_add = calloc(1, sizeof(*to_add));
  to_add->val = val;
  to_add->val_len = strlen(val);
  to_add->next = NULL;
  to_add->prev = list->last;
  if (list->last) {
    list->last->next = to_add;
    list->last = to_add;
  } else {
    list->first = to_add;
    list->last = to_add;
  }

  if (list->path_len != 1) {
    list->path_len += 1 + to_add->val_len;
  } else {
    list->path_len += to_add->val_len;
  }
  ++list->nodes_cnt;
}

void destroy_path_list(PathList *list) {
  if (list == NULL) {
    return;
  }

  PathNode *cur = list->first;
  while (cur) {
    PathNode *nxt = cur->next;
    free(cur->val);
    free(cur);
    cur = nxt;
  }
  free(list);
}