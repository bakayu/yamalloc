# yamalloc

Yet Another Memory Allocator is an educational C library for exploring how dynamic memory allocators manage memory. The project favors understandable design and correctness over performance.

## Project proposal

### Project description

`yamalloc` is a small allocator library that obtains memory from the operating system and divides it into blocks that programs can allocate and release. It provides an interface inspired by `malloc` and `free`, while keeping the allocator implementation small enough to study.

The initial implementation uses `mmap` to obtain a region of memory, stores metadata for each block, and searches the block list using first fit.

### Goals

- Learn how allocators request, divide, reuse, and release memory.
- Provide a small C API for allocating and freeing memory.
- Implement first-fit block selection, splitting, and coalescing.
- Add tests for allocation, freeing, reuse, alignment, and exhaustion.
- Document how to build and use the library, and how its design works.

### Specifications

#### public interface

```c
void *heap_alloc(size_t size);
void heap_free(void *ptr);
```

The allocator should return suitably aligned memory for ordinary C objects. Allocation failure should be reported with `NULL`; freeing `NULL` should do nothing, and zero-allocations should also return `NULL`.

`print_allocator_state` is a debug helper to check the current state of the allocator.

#### Current implementation limits

The current allocator maps one system page and manages blocks inside it (typically `4096B` on a `64bit` system). It does not yet grow the region when it runs out of space, so large allocations or enough smaller allocations can exhaust it.

### Design

#### Memory source and block layout

The allocator obtains a region from the operating system with `mmap`. It stores a header before each payload. A header records the payload size, whether the block is free, and the next block in address order. Payload sizes and header locations are aligned so returned pointers are suitable for C objects.

```plaintext
[header][payload][header][payload] ...
```

#### Allocation and freeing

Allocation walks the block list from its beginning and chooses the first free block large enough for the aligned request. If the block has enough room, it is split into an allocated block and a new free block. Freeing marks a block available again. Adjacent free blocks can be coalesced to reduce fragmentation while freeing.

The list contains allocated and free blocks in address order. This is an implicit free-list design: finding a free block may require walking allocated blocks too.

#### Other allocator designs to compare

- **Linear or stack allocator:** advances a pointer for each allocation. It is very fast, but individual allocations usually cannot be freed, only the last allocations can be freed without loosing data.
- **Arena allocator:** obtains a region and serves allocations from it, often with linear allocation. It is useful when many objects share a lifetime and can all be released together. “Arena” describes a lifetime and ownership model; an arena can use different allocation strategies internally.
- **General-purpose heap allocator:** supports allocations with different sizes and lifetimes, along with individual frees. It must track free space and manage fragmentation. This library implements a heap allocator.
- **Pool or slab allocator:** manages slots of one size or a small set of sizes. It can make repeated same-size allocations efficient, at the cost of being less general or using multiple pools.

## Usage guide

### Build

Build the static library with:

```sh
make
```

This creates `build/libyamalloc.a`.

### Use the library

Include the public header and link the archive when compiling your program:

```c
#include "yamalloc.h"

int main(void) {
    int *arr = heap_alloc(10 * sizeof(int));
    if (arr == NULL) {
        return 1;
    }

    arr[0] = 42;
    heap_free(values);
    return 0;
}
```

### Run tests

Run every test program in `tests/` with:

```sh
make test
```

Run one test by filename with:

```sh
make run smoke.c
```

### Inspect allocator state

For debugging, call `print_allocator_state()` from a test or example after allocating or freeing blocks. It prints the allocator’s current block list debug info.
