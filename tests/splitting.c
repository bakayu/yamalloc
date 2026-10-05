#include "yamalloc.h"
#include <stdio.h>

int main() {
    printf("1. Block splitting test:\n");

    unsigned char *first = (unsigned char *)heap_alloc(128);
    unsigned char *second = (unsigned char *)heap_alloc(128);
    if (first == NULL || second == NULL) {
        printf("FAILED: allocation returned NULL\n");
        heap_free(first);
        heap_free(second);
        return 1;
    }

    first[0] = 'A';
    first[127] = 'B';
    second[0] = 'C';
    second[127] = 'D';

    if (first[0] != 'A' || first[127] != 'B' ||
        second[0] != 'C' || second[127] != 'D') {
        printf("FAILED: writing one block changed the other\n");
        heap_free(first);
        heap_free(second);
        return 1;
    }

    printf("-- Both allocations remain independent\n");
    heap_free(first);
    heap_free(second);
    return 0;
}
