#ifndef H_PATH_LIST
#define H_PATH_LIST

#include <stddef.h>

typedef struct PathNode {
  char *val;
  size_t val_len; // without '\0'
  struct PathNode *prev;
  struct PathNode *next;
} PathNode;

typedef struct PathList {
  PathNode *first;
  PathNode *last;
  size_t path_len;
  size_t nodes_cnt;
} PathList;

// Returns joined path allocated on the heap.
char *join_path(const PathList *list);

// Returns splitted path allocated on the heap.
PathList *split_path(const char *path);

PathNode *pop_back_no_free(PathList *list);
void pop_back(PathList *list);
void free_node(PathNode *node);

void push_back(PathList *list, char *val);
void push_back_node(PathList *list, PathNode *node);

void destroy_path_list(PathList *list);

#endif //  H_PATH_LIST