#define _GNU_SOURCE

#include "filesystem.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "path_list.h"

enum { NCURSES_WINDOW_RIGHT_SIZE_OFFSET = 1 };

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

static off_t get_file_size(const char *dir_path, const FilesystemEntity *fs_ent,
                           size_t fs_ent_name_len) {
  PathList *path_list = split_path(dir_path);

  char *fs_ent_name = calloc(1, fs_ent_name_len + 1);
  if (fs_ent_name == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }
  strcpy(fs_ent_name, fs_ent->name);
  push_back(path_list, fs_ent_name);

  char *filepath = join_path(path_list);
  destroy_path_list(path_list);
  struct stat st;
  if (stat(filepath, &st) != 0) {
    perror("stat:");
    debug_print("%s %s\n", "can't get stat for file", filepath);
    free(filepath);
    return -1;
  }
  free(filepath);
  return st.st_size;
}

static char *get_file_descr(const char *dir_path,
                            const FilesystemEntity *fs_ent, unsigned len,
                            size_t fs_ent_name_len) {
  off_t filesize = get_file_size(dir_path, fs_ent, fs_ent_name_len);
  int filesize_len = snprintf(NULL, 0, "%jd", filesize);

  char *buf = calloc(1, len + 1);
  if (buf == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }

  int padding =
      (int)len - fs_ent_name_len - 2 - NCURSES_WINDOW_RIGHT_SIZE_OFFSET;
  if (padding <= 0) {
    snprintf(buf, len + 1, "%s", fs_ent->name);
  } else {
    snprintf(buf, len + 1, "%s%*jd %c", fs_ent->name, padding, filesize,
             (char)fs_ent->entity_type);
  }
  return buf;
}

static char *get_dir_descr(const char *dir_path, const FilesystemEntity *fs_ent,
                           unsigned len, size_t fs_ent_name_len) {
  char *buf = calloc(1, len + 1);
  if (buf == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }

  int padding = (int)len - fs_ent_name_len - NCURSES_WINDOW_RIGHT_SIZE_OFFSET;
  if (padding <= 0) {
    snprintf(buf, len + 1, "%s", fs_ent->name);
  } else {
    snprintf(buf, len + 1, "%s%*c", fs_ent->name, padding,
             (char)fs_ent->entity_type);
  }
  return buf;
}

static char *get_symlink_descr(const char *dir_path,
                               const FilesystemEntity *fs_ent, unsigned len,
                               size_t fs_ent_name_len) {
  char *buf = calloc(1, len + 1);
  if (buf == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }

  int padding = (int)len - fs_ent_name_len - NCURSES_WINDOW_RIGHT_SIZE_OFFSET;
  if (padding <= 0) {
    snprintf(buf, len + 1, "%s", fs_ent->name);
  } else {
    snprintf(buf, len + 1, "%s%*c", fs_ent->name, padding,
             (char)fs_ent->entity_type);
  }
  return buf;
}

static char *get_other_descr(const FilesystemEntity *fs_ent, unsigned len,
                             size_t fs_ent_name_len) {
  char *buf = calloc(1, len + 1);
  if (buf == NULL) {
    debug_print("%s\n", "can't alloc");
    exit(EXIT_FAILURE);
  }
  snprintf(buf, len + 1, "%s", fs_ent->name);
  return buf;
}

static char *get_fs_ent_descr(const char *dir_path,
                              const FilesystemEntity *fs_ent, unsigned len) {
  size_t name_len = strlen(fs_ent->name);
  if (fs_ent->entity_type == ET_FILE) {
    return get_file_descr(dir_path, fs_ent, len, name_len);
  } else if (fs_ent->entity_type == ET_DIRECTORY) {
    return get_dir_descr(dir_path, fs_ent, len, name_len);
  } else if (fs_ent->entity_type == ET_SYMLINK) {
    return get_symlink_descr(dir_path, fs_ent, len, name_len);
  }
  return get_other_descr(fs_ent, len, name_len);
}

static bool is_parent_directory_name(const char *name) {
  return strlen(name) == 2 && name[0] == '.' && name[1] == '.';
}

static bool is_hidden(bool is_dir, const char *filename) {
  if (is_dir && is_parent_directory_name(filename)) {
    return false;
  }
  return filename[0] == '.';
}

FilesystemEntity *get_filesystem_entities(const char *path, size_t *cnt,
                                          bool show_hidden,
                                          unsigned descr_length) {
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
    if (!show_hidden && is_hidden(dp->d_type == DT_DIR, dp->d_name)) {
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
    } else if (dp->d_type == DT_LNK) {
      buf[size].entity_type = ET_SYMLINK;
    } else {
      buf[size].entity_type = ET_OTHER;
    }
    buf[size].descr = get_fs_ent_descr(path, &buf[size], descr_length);
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
  return is_parent_directory_name(fs_ent->name);
}

void destroy_fs_entities(FilesystemEntity *fs_ent, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    free(fs_ent[i].descr);
  }
  free(fs_ent);
}

static int remove_callback(const char *filepath, const struct stat *sb,
                           int typeflag, struct FTW *ftwbuf) {
  int rv = remove(filepath);
  if (rv) {
    perror(filepath);
  }
  return rv;
}

int recursive_delete(char *path, enum EntityType en_type) {
  int typeflag;
  if (en_type == ET_DIRECTORY) {
    typeflag = FTW_D;
  } else {
    typeflag = FTW_F;
  }
  return nftw(path, remove_callback, typeflag, FTW_DEPTH | FTW_PHYS);
}