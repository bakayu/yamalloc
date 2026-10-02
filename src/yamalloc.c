#include "yamalloc.h"
#include "internals.h"

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define ALIGNMENT _Alignof(max_align_t)
#define MIN_SPLIT_SIZE ALIGNMENT

static heapinfo_t heap;
static bool heap_initialized;

static size_t header_size(void) {
    size_t size = sizeof(heapchunk_t);
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

static size_t align_size(size_t size) {
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

int PROT_FLAGS = PROT_READ | PROT_WRITE;
int MAP_FLAGS = MAP_ANONYMOUS | MAP_PRIVATE;

heap_e heap_init(void) {
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0 || (size_t)page_size <= header_size()) {
        return HEAP_INIT_ERROR;
    }

    // allocate memory from kernel for the heap
    size_t mapping_size = (size_t)page_size;
    void *start = mmap(NULL, mapping_size, PROT_FLAGS, MAP_FLAGS, -1, 0);
    if (start == (void *)-1) {
        perror("mmap");
        return HEAP_INIT_ERROR;
    }

    heapchunk_t *first = (heapchunk_t *)(start);
    first->size = mapping_size - header_size();
    first->free = true;
    first->next = NULL;

    heap.start = first;
    heap.avail = first->size;
    heap.mapped_size = mapping_size;
    heap_initialized = true;

    return HEAP_INIT_OK;
}
