#ifndef H_FILESYSTEM
#define H_FILESYSTEM

#include <limits.h>
#include <stdbool.h>

#include "logging.h"

enum EntityType { ET_FILE = 0, ET_DIRECTORY = 1, ET_OTHER = 2 };

typedef struct FilesystemEntity {
  char name[NAME_MAX + 1];
  enum EntityType entity_type;

} FilesystemEntity;

// returns array of FilesystemEntity objects
// puts array size into cnt
FilesystemEntity *get_filesystem_entities(const char *path, size_t *cnt);

bool is_parent_directory(const FilesystemEntity *fs_ent);

#endif // H_FILESYSTEM