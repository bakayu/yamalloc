#include "yamalloc.h"
#include <stddef.h>
#include <stdio.h>

#define CAPACITY 640000

char heap[CAPACITY] = {0};

void *heap_alloc(size_t size) { return NULL; }

void heap_free(void *ptr) {}

void heap_collect() {}
