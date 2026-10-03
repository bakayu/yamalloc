#include "yamalloc.h"
#include "internals.h"

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define ALIGNMENT _Alignof(max_align_t)
#define MIN_SPLIT_SIZE ALIGNMENT

static heapinfo_t heap;
static bool heap_initialized;

static size_t header_size(void) {
    size_t size = sizeof(heapchunk_t);
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

static size_t align_size(size_t size) {
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

int PROT_FLAGS = PROT_READ | PROT_WRITE;
int MAP_FLAGS = MAP_ANONYMOUS | MAP_PRIVATE;

heap_e heap_init(void) {
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0 || (size_t)page_size <= header_size()) {
        return HEAP_INIT_ERROR;
    }

    // allocate memory from kernel for the heap
    size_t mapping_size = (size_t)page_size;
    void *start = mmap(NULL, mapping_size, PROT_FLAGS, MAP_FLAGS, -1, 0);
    if (start == (void *)-1) {
        perror("mmap");
        return HEAP_INIT_ERROR;
    }

    heapchunk_t *first = (heapchunk_t *)(start);
    first->size = mapping_size - header_size();
    first->free = true;
    first->next = NULL;

    heap.start = first;
    heap.avail = first->size;
    heap.mapped_size = mapping_size;
    heap_initialized = true;

    return HEAP_INIT_OK;
}

static void split_chunk(heapchunk_t *chunk, size_t size) {
    size_t overhead = header_size();

    if (chunk->size < size + overhead + MIN_SPLIT_SIZE) {
        return;
    }

    unsigned char *payload = (unsigned char *)chunk + overhead;
    heapchunk_t *remainder = (heapchunk_t *)(payload + size);

    remainder->size = chunk->size - size - overhead;
    remainder->free = true;
    remainder->next = chunk->next;

    chunk->size = size;
    chunk->next = remainder;
}

void *heap_alloc(size_t size) {
    if (size == 0 || size > SIZE_MAX - (ALIGNMENT - 1)) {
        return NULL;
    }

    size = align_size(size);
    if (!heap_initialized && heap_init() == HEAP_INIT_OK) {
        return NULL;
    }

    for (heapchunk_t *chunk = heap.start; chunk != NULL; chunk = chunk->next) {
        if (!chunk->free || chunk->size < size) {
            continue;
        }

        size_t original_size = chunk->size;
        split_chunk(chunk, size);
        chunk->free = false;

        heap.avail -= original_size - chunk->size;

        return (unsigned char *)chunk + header_size();
    }

    return NULL;
}

void heap_free(void *ptr) {
    if (ptr == NULL || !heap_initialized) {
        return;
    }

    for (heapchunk_t *chunk = heap.start; chunk != NULL; chunk = chunk->next) {
        void *payload = (unsigned char *)chunk + header_size();

        if (payload != ptr) {
            continue;
        }

        if (chunk->free) {
            return;
        }

        chunk->free = true;
        heap.avail += chunk->size;

        heapchunk_t *current = heap.start;
        while (current != NULL && current->next != NULL) {
            heapchunk_t *next = current->next;
            unsigned char *current_end =
                (unsigned char *)current + header_size() + current->size;

            if (current->free && next->free &&
                current_end == (unsigned char *)next) {
                current->size += header_size() + next->size;
                current->next = next->next;
                heap.avail += header_size();
            } else {
                current = next;
            }
        }

        return;
    }
}
