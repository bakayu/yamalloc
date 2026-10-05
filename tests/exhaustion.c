#include "yamalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("1. Heap exhaustion test:\n");

    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        printf("FAILED: could not determine page size\n");
        return 1;
    }

    size_t capacity = (size_t)page_size / sizeof(void *) + 1;
    void **blocks = (void **)malloc(capacity * sizeof(void *));
    if (blocks == NULL) {
        printf("FAILED: could not allocate test bookkeeping\n");
        return 1;
    }

    size_t count = 0;
    while (count < capacity) {
        blocks[count] = heap_alloc(1);
        if (blocks[count] == NULL) {
            break;
        }
        count++;
    }

    if (count == 0 || count == capacity) {
        printf("FAILED: allocator did not report exhaustion as expected\n");
        for (size_t i = 0; i < count; i++) {
            heap_free(blocks[i]);
        }
        free(blocks);
        return 1;
    }

    printf("-- Allocator returned NULL after %zu allocations\n", count);
    for (size_t i = 0; i < count; i++) {
        heap_free(blocks[i]);
    }
    free(blocks);
    return 0;
}
