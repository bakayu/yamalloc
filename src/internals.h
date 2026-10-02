#ifndef YAMALLOC_INTERNALS_H
#define YAMALLOC_INTERNALS_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    HEAP_INIT_OK,
    HEAP_INIT_ERROR,
} heap_e;

typedef struct heapchunk_t {
    size_t size;
    bool free;
    struct heapchunk_t *next;
} heapchunk_t;

typedef struct heapinfo_t {
    struct heapchunk_t *start;
    size_t avail;
    size_t mapped_size;
} heapinfo_t;

heap_e heap_init(void);

#endif
