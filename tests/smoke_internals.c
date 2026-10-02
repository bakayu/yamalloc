#include <internals.h>
#include <stdio.h>

int main(void) {
    heapinfo_t heap = {0};
    if (heap_init() == HEAP_INIT_ERROR) {
        printf("Failed to init heap.");
        return -1;
    }

    return 0;
}
