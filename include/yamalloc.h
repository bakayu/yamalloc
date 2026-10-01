#ifndef YAMALLOC_H
#define YAMALLOC_H

#include <stdint.h>

void *heap_alloc(uint32_t size);
void heap_free(void *ptr);

#endif
