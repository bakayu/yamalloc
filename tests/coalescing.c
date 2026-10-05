#include "yamalloc.h"
#include <stdio.h>

int main() {
    printf("1. Adjacent block coalescing test:\n");

    void *first = heap_alloc(128);
    void *second = heap_alloc(128);
    void *barrier = heap_alloc(128);
    if (first == NULL || second == NULL || barrier == NULL) {
        printf("FAILED: allocation returned NULL\n");
        heap_free(first);
        heap_free(second);
        heap_free(barrier);
        return 1;
    }

    heap_free(first);
    heap_free(second);

    void *combined = heap_alloc(256);
    if (combined != first) {
        printf("FAILED: adjacent free blocks were not coalesced\n");
        heap_free(combined);
        heap_free(barrier);
        return 1;
    }

    printf("-- Adjacent blocks coalesced and were reused\n");
    heap_free(combined);
    heap_free(barrier);
    return 0;
}
