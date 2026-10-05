#include "yamalloc.h"
#include <stdio.h>

int main() {
    printf("1. Freed block reuse test:\n");

    void *first = heap_alloc(128);
    void *second = heap_alloc(128);
    if (first == NULL || second == NULL) {
        printf("FAILED: allocation returned NULL\n");
        heap_free(first);
        heap_free(second);
        return 1;
    }

    heap_free(first);
    void *reused = heap_alloc(64);
    if (reused != first) {
        printf("FAILED: allocator did not reuse the freed block\n");
        heap_free(reused);
        heap_free(second);
        return 1;
    }

    printf("-- Reused the first freed block\n");
    heap_free(reused);
    heap_free(second);
    return 0;
}
