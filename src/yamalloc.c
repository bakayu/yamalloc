#include "yamalloc.h"
#include "internals.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int PROT_FLAGS = PROT_READ | PROT_WRITE;
int MAP_FLAGS = MAP_ANONYMOUS | MAP_PRIVATE;

static heapinfo_t *heap = NULL;

heap_e heap_init() {
    // allocate memory from kernel for the heap
    void *start = mmap(NULL, getpagesize(), PROT_FLAGS, MAP_FLAGS, -1, 0);
    if (start == (void *)-1) {
        perror("mmap");
        return HEAP_INIT_ERROR;
    }

    printf("%p\n", start);

    heapchunk_t *first = (heapchunk_t *)(start);

    first->size = getpagesize() - sizeof(heapchunk_t);
    first->free = true;
    first->next = NULL;

    heap->start = first;

    return HEAP_INIT_OK;
}
