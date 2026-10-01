#include <stdbool.h>
#include <stdint.h>

typedef enum {
    HEAP_INIT_OK,
    HEAP_INIT_ERROR,
} heap_e;

struct heapchunk_t {
    uint32_t size;
    bool free;
    struct heapchunk_t *next;
};

struct heapinfo_t {
    struct heapchunk_t *start;
    uint32_t avail;
};

heap_e heap_init(struct heapinfo_t *heap);
