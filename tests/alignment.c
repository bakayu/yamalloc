#include "yamalloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    printf("1. Allocation alignment test:\n");

    void *ptr = heap_alloc(1);
    if (ptr == NULL) {
        printf("FAILED: allocation returned NULL\n");
        return 1;
    }

    if ((uintptr_t)ptr % _Alignof(max_align_t) != 0) {
        printf("FAILED: allocation is not suitably aligned\n");
        heap_free(ptr);
        return 1;
    }

    printf("-- Allocation is aligned to %zu bytes\n",
           (size_t)_Alignof(max_align_t));
    heap_free(ptr);
    return 0;
}
