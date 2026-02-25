# Heap Memory Manager (HMM)

A custom heap memory manager written in C that replaces the standard `malloc`, `free`, `calloc`, and `realloc` with a first-fit allocator backed by `sbrk`.

---

## Table of Contents

- [Overview](#overview)
- [Memory Layout](#memory-layout)
- [Free List Design](#free-list-design)
- [API Reference](#api-reference)
- [Internal Helper Functions](#internal-helper-functions)
- [Allocation Strategy](#allocation-strategy)
- [Coalescing & Program-Break Lowering](#coalescing--program-break-lowering)
- [Building](#building)
- [Running Tests](#running-tests)
- [Known Limitations](#known-limitations)

---

## Overview

HMM manages a pool of heap memory obtained from the operating system through `sbrk()`. It maintains a **doubly-linked free list** sorted by ascending address so that adjacent-block detection is O(1). When a call to `HmmAlloc` cannot be satisfied from the existing free list, the heap is grown automatically in chunks of 10 blocks.

Key features:

- First-fit allocation with block splitting
- Immediate coalescing (merge on every `HmmFree`)
- Program-break lowering when the top-of-heap block is large enough
- Drop-in `malloc`/`free`/`calloc`/`realloc` overrides
- Double-free and `free(NULL)` protection
- 8-byte alignment on all returned pointers

---

## Memory Layout

Every region managed by HMM starts with a small header followed by user-accessible data.

**Free block:**

```
+------------------+  <-- FreeBlock_t*
| block_length     |  (size_t)  bytes of usable data
| prev_free*       |  (FreeBlock_t*)
| next_free*       |  (FreeBlock_t*)
| ... data ...     |  block_length bytes
+------------------+
```

**Allocated block:**

```
+------------------+  <-- AllocatedBlock_t*
| block_length     |  (size_t)  bytes of usable data
| ... data ...     |  block_length bytes  <-- user pointer returned here
+------------------+
```

`META_DATA_SIZE = sizeof(AllocatedBlock_t) = sizeof(size_t)`.

The user pointer is always `header + META_DATA_SIZE`. Recovering the header from a user pointer is `userptr - META_DATA_SIZE`.

Because a free block needs room for two extra pointers inside the data area, the minimum usable size of a block on the free list is `sizeof(FreeBlock_t) - META_DATA_SIZE` bytes (i.e., two pointer widths).

---

## Free List Design

The free list is a doubly-linked list kept in **ascending address order**. This invariant is maintained by `freelist_insert_sorted()` on every insertion. Sorted order gives two benefits:

1. `merge_adjacent()` only needs to look at the immediate neighbours of a newly freed block (not traverse the whole list) to determine physical contiguity.
2. `LowerProgramBreak()` can verify the candidate block is truly at the program break without an extra `sbrk(0)` traversal on every free.

---

## API Reference

### `void Hmm_init(void)`

Pre-populates the free list by calling `sbrk(DEFAULT_MEM_ALLOC)` and carving the result into blocks of `DEFAULT_BLOCK_LENGTH` bytes each. Call this once before any allocations.

---

### `void *HmmAlloc(size_t size)`

Allocates at least `size` bytes. Returns a pointer to the allocated memory, or `NULL` on failure. The returned pointer is always 8-byte aligned.

---

### `void HmmFree(void *ptr)`

Frees a previously allocated block. Safe to call with `NULL` (no-op). Silently ignores double-free attempts. After freeing, immediately attempts to coalesce adjacent free blocks and lower the program break.

---

### `void *HmmRealloc(void *ptr, size_t size)`

- `realloc(NULL, n)` behaves like `HmmAlloc(n)`.
- `realloc(ptr, 0)` behaves like `HmmFree(ptr)` and returns `NULL`.
- If `size <= current block size`, the original pointer is returned unchanged (shrink-in-place).
- Otherwise a new block is allocated, data is copied with `memcpy`, and the old block is freed.

---

### `void *HmmCalloc(size_t nmemb, size_t size)`

Allocates `nmemb * size` zero-initialised bytes. Includes overflow protection for the multiplication.

---

## Internal Helper Functions

| Function | Purpose |
|---|---|
| `align8(size)` | Round size up to the nearest 8-byte multiple |
| `freelist_insert_sorted(block)` | Insert a block into the free list maintaining address order |
| `freelist_remove(block)` | Unlink a block from the free list |
| `allocate_block(block, size)` | Remove from free list, set header, return user pointer |
| `split_block(block, size)` | Carve a right-hand piece of exactly `size` bytes from a larger block |
| `merge_adjacent(block)` | Coalesce with left/right neighbours if physically contiguous |
| `grow_heap(size)` | `sbrk` 10 new blocks and add them to the free list |
| `LowerProgramBreak()` | Release the top-of-heap free block back to the OS if it is large enough |

---

## Allocation Strategy

`HmmAlloc` uses a **first-fit** strategy:

1. Walk the free list from the head.
2. The first block with `block_length >= size` is selected.
3. `split_block` attempts to carve a right-hand piece of exactly `size` bytes, leaving the remainder on the free list. If the remainder would be too small to hold free-list pointers, the entire block is given to the caller instead.
4. If no suitable block is found, `grow_heap` is called. It acquires 10 blocks worth of memory from the OS, adds them all to the sorted free list, and then the same split logic applies to the first of those blocks.

---

## Coalescing & Program-Break Lowering

**Coalescing** (`merge_adjacent`) runs inside `HmmFree` immediately after a block is reinserted into the sorted free list. Because the list is address-ordered, the newly freed block's physical neighbours are guaranteed to be its immediate list neighbours (if they are free at all). The function checks both directions in O(1).

**Program-break lowering** (`LowerProgramBreak`) runs after every coalesce. It finds the free block at the highest address, confirms it ends exactly at the current program break (`sbrk(0)`), and if the block is at least `DEFAULT_LOWERING_VAL` bytes total, removes it from the free list and calls `sbrk(-n)` to return the memory to the OS.

---

## Building

```bash
# Build the main demo
make build

# Clean all build artifacts
make clean
```

The `Makefile` compiles `Hmm.c` and `project.c` into `FINAL.out`.

---

## Running Tests

```bash
gcc -g -Wall -o test_hmm Hmm.c tests/test_main.c -I.
./test_hmm
```

Expected output ends with:

```
=== Results: 41 passed, 0 failed ===
```

The test suite covers: basic alloc/free, double-free safety, free(NULL), 8-byte alignment, 20 concurrent allocations, large allocations (heap growth), calloc zero-initialisation, realloc edge cases (NULL, size 0, grow), and non-overlapping allocation verification.

---

## Known Limitations

- **Not thread-safe.** There is no locking around the free list. Do not use from multiple threads without adding a mutex around all public API calls.
- **No boundary tags.** Coalescing works only between blocks that are currently on the free list; a freed block cannot merge with an allocated neighbour even if it is physically adjacent.
- **First-fit only.** Best-fit or segregated-fit would reduce fragmentation for workloads with many different allocation sizes.
- **`sbrk` is deprecated on some platforms** (macOS since 10.12, not available on Windows). A portable alternative would use `mmap`/`VirtualAlloc`.
- **Double-free detection is O(n).** The guard traverses the entire free list on every `HmmFree`. In production this would be replaced with a canary or red-zone in the block header.
