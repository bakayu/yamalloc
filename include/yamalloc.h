#include <stddef.h>

#define CAPACITY 640000

void *heap_alloc(size_t size);

void heap_free(void *ptr);

void heap_collect();
