#include "yamalloc.h"
#include "internals.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

heap_e heap_init(struct heapinfo_t *heap) {
    // allocate memory from kernel for the heap
    void *start = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
                       MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (start == (void *)-1) {
        perror("mmap");
        return HEAP_INIT_ERROR;
    }

    printf("%p\n", start);

    struct heapchunk_t *first = (struct heapchunk_t *)(start);

    first->size = getpagesize() - sizeof(struct heapchunk_t);
    first->free = true;
    first->next = NULL;

    heap->start = first;

    return HEAP_INIT_OK;
}
