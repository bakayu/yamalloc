#include "yamalloc.h"
#include <stdio.h>

int main() {
    printf("1. Zero-size allocation and repeated free test:\n");

    if (heap_alloc(0) != NULL) {
        printf("FAILED: zero-size allocation should return NULL\n");
        return 1;
    }
    printf("-- Zero-size allocation returned NULL\n");

    void *ptr = heap_alloc(64);
    if (ptr == NULL) {
        printf("FAILED: allocation returned NULL\n");
        return 1;
    }

    heap_free(ptr);
    heap_free(ptr);

    void *reused = heap_alloc(64);
    if (reused != ptr) {
        printf("FAILED: repeated free affected reuse of the block\n");
        heap_free(reused);
        return 1;
    }

    printf("-- Repeated free was ignored and the block remained reusable\n");
    heap_free(reused);
    return 0;
}
