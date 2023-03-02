#define _GNU_SOURCE

#include "filesystem.h"

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static FilesystemEntity* resize(FilesystemEntity* buf, size_t new_capacity) {
    FilesystemEntity* new_buf = realloc(buf, sizeof(*buf) * new_capacity);
    if (new_buf == NULL) {
        debug_print("%s\n", "can't realloc");
        exit(EXIT_FAILURE);
    }
    return new_buf;
}

static bool is_dot(char fname[256]) {
    return strlen(fname) == 1 && fname[0] == '.';
}

FilesystemEntity* get_filesystem_entities(const char* path, size_t* cnt) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        debug_print("%s\n", "can't opendir");
        exit(EXIT_FAILURE);
    }

    size_t capacity = 1;
    size_t size = 0;
    FilesystemEntity* buf = calloc(capacity, sizeof(*buf));
    if (buf == NULL) {
        debug_print("%s\n", "can't alloc");
        exit(EXIT_FAILURE);
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
        ++size;
    }
    closedir(dir);

    buf = resize(buf, size);
    *cnt = size;
    return buf;
}