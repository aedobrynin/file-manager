#ifndef H_FILESYSTEM
#define H_FILESYSTEM

#include <limits.h>
#include <stdbool.h>

#include "logging.h"

enum EntityType {
  ET_FILE = 'F',
  ET_DIRECTORY = 'D',
  ET_OTHER = 'O',
  ET_SYMLINK = 'S'
};

typedef struct FilesystemEntity {
  char name[NAME_MAX + 1];
  char *descr;
  enum EntityType entity_type;
} FilesystemEntity;

// returns array of FilesystemEntity objects
// puts array size into cnt
FilesystemEntity *get_filesystem_entities(const char *path, size_t *cnt,
                                          bool show_hidden,
                                          unsigned descr_width);

bool is_parent_directory(const FilesystemEntity *fs_ent);

void destroy_fs_entities(FilesystemEntity *fs_ent, size_t sz);

#endif // H_FILESYSTEM