#ifndef H_LOGGING
#define H_LOGGING

#ifndef DEBUG
#define DEBUG 0
#endif

#include <stdio.h>

#define debug_print(fmt, ...)                                                  \
  do {                                                                         \
    if (DEBUG) {                                                               \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__,        \
              __VA_ARGS__);                                                    \
    }                                                                          \
  } while (0)

#endif //  H_LOGGING