#ifndef H_PATH_LIST
#define H_PATH_LIST
 
typedef struct PathNode {
    const char* value;
    struct PathNode* prev;
    struct PathNode* next;
} PathNode;

typedef struct PathList {
    PathNode* first;
    PathNode* last;
} PathList;

// Returns joined path allocated on the heap.
char* join_path(const PathList* list);

// Returns splitted path allocated on the heap.
PathList* split_path(const char* path);

void pop_back(PathList* list);

void push_back(PathList* list);

void free_path_list(PathList * list);

#endif