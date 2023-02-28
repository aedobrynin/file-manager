#include "path_list.h"


char* join_path(const PathList* list);

PathList* split_path(const char* path);

void pop_back(PathList* list);

void push_back(PathList* list);

void free_path_list(PathList * list);