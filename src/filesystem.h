#ifndef H_FILESYSTEM
#define H_FILESYSTEM

#include <limits.h>

#include "logging.h"

typedef struct FilesystemEntity {
    char name[NAME_MAX + 1];
} FilesystemEntity;

// returns array of FilesystemEntity objects
// puts array size into cnt
FilesystemEntity* get_filesystem_entities(const char* path, size_t* cnt);

#endif // H_FILESYSTEM