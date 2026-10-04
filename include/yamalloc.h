#ifndef YAMALLOC_H
#define YAMALLOC_H

#include <stddef.h>

void *heap_alloc(size_t size);
void heap_free(void *ptr);
void print_allocator_state(void);

#endif
