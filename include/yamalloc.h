#ifndef YAMALLOC_H
#define YAMALLOC_H

#include <stddef.h>

void *heap_alloc(size_t size);
void heap_free(void *ptr);

#endif
