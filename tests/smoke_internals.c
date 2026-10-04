#include <internals.h>
#include <stdio.h>

int main(void) {
    if (heap_init() == HEAP_INIT_ERROR) {
        printf("Failed to init heap.");
        return -1;
    }

    printf("heap initialized.\n");

    return 0;
}
