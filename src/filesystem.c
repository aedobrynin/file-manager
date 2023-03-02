#define _GNU_SOURCE

#include "filesystem.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static FilesystemEntity *resize(FilesystemEntity *buf, size_t new_capacity) {
  FilesystemEntity *new_buf = realloc(buf, sizeof(*buf) * new_capacity);
  if (new_buf == NULL) {
    debug_print("%s\n", "can't realloc");
    exit(EXIT_FAILURE);
  }
  return new_buf;
}

static bool is_dot(char fname[256]) {
  return strlen(fname) == 1 && fname[0] == '.';
}

static int sort_fs_entities_by_name(const void *a, const void *b) {
  const FilesystemEntity *fs_a = a;
  const FilesystemEntity *fs_b = b;
  return strncmp(fs_a->name, fs_b->name, NAME_MAX + 1);
}

FilesystemEntity *get_filesystem_entities(const char *path, size_t *cnt) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    debug_print("%s\n", "can't opendir");
    return NULL;
  }

  size_t capacity = 1;
  size_t size = 0;
  FilesystemEntity *buf = calloc(capacity, sizeof(*buf));
  if (buf == NULL) {
    debug_print("%s\n", "can't alloc");
    return NULL;
  }
  struct dirent *dp;
  while ((dp = readdir(dir)) != NULL) {
    if (is_dot(dp->d_name)) {
      continue;
    }
    if (size == capacity) {
      enum {
        REALLOC_FACTOR = 2,
      };
      capacity *= REALLOC_FACTOR;
      buf = resize(buf, capacity);
    }
    strncpy(buf[size].name, dp->d_name, 256);
    if (dp->d_type == DT_REG) {
      buf[size].entity_type = ET_FILE;
    } else if (dp->d_type == DT_DIR) {
      buf[size].entity_type = ET_DIRECTORY;
    } else {
      buf[size].entity_type = ET_OTHER;
    }
    ++size;
  }
  closedir(dir);

  buf = resize(buf, size);
  *cnt = size;
  qsort(buf, size, sizeof(*buf), &sort_fs_entities_by_name);
  return buf;
}

bool is_parent_directory(const FilesystemEntity *fs_ent) {
  if (fs_ent->entity_type != ET_DIRECTORY) {
    return false;
  }
  return strlen(fs_ent->name) == 2 && fs_ent->name[0] == '.' &&
         fs_ent->name[1] == '.';
}