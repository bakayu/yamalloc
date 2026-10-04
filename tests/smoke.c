#include "yamalloc.h"
#include <stdio.h>

int main() {
    printf("Testing custom allocator...\n\n");

    // Test basic allocation and freeing
    printf("1. Basic allocation test:\n");
    int *array = (int *)heap_alloc(10 * sizeof(int));
    if (array) {
        for (int i = 0; i < 10; i++) {
            array[i] = i * i;
        }
        printf("-- Allocated and filled array\n");
        print_allocator_state();

        heap_free(array);
        printf("-- Freed array\n");
        print_allocator_state();
    }

    // Test multiple allocations
    printf("\n2. Multiple allocation test:\n");
    char *str1 = (char *)heap_alloc(100);
    char *str2 = (char *)heap_alloc(200);
    char *str3 = (char *)heap_alloc(50);

    printf("-- Allocated three blocks\n");
    print_allocator_state();

    heap_free(str1);
    printf("-- Freed first block\n");
    print_allocator_state();

    heap_free(str3);
    printf("-- Freed third block\n");
    print_allocator_state();

    heap_free(str2);
    printf("-- Freed second block\n");
    print_allocator_state();

    // Test large allocation
    printf("\n3. Large allocation test:\n");
    // NOTE: since we use mmap to map single page, our allocator has 4096KB,
    // of data to work with, the max allocation we can do is 4064KB.
    // 4096KB - HEADER SIZE => 4096 - 32 = 4064KB
    void *large = heap_alloc(4064);
    if (large == NULL)
        printf("FAILED\n");
    printf("-- Allocated large block\n");
    print_allocator_state();

    heap_free(large);
    printf("-- Freed large block\n");
    print_allocator_state();

    printf("All tests completed!\n");
    return 0;
}
