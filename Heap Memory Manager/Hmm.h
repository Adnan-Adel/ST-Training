#ifndef _HMM_H_
#define _HMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* -----------------------------------------------------------------------
 * Configuration
 * ----------------------------------------------------------------------- */
#define DEFAULT_MEM_ALLOC    (10 * 1024 + 10 * sizeof(FreeBlock_t))
#define DEFAULT_BLOCK_LENGTH (1 * 1024)
#define DEFAULT_LOWERING_VAL (300 * 1024)

/*
 * META_DATA_SIZE is the header overhead for ANY block (free or allocated).
 * We use sizeof(AllocatedBlock_t) == sizeof(size_t) as the common header.
 * Free blocks additionally carry two pointers, but the user-facing offset
 * is always META_DATA_SIZE.
 */
#define META_DATA_SIZE  sizeof(AllocatedBlock_t)

/* -----------------------------------------------------------------------
 * Block structures
 *
 *  Memory layout (free block):
 *    +------------------+  <-- FreeBlock_t* / AllocatedBlock_t*
 *    | block_length     |  (size_t)
 *    | prev_free        |  (FreeBlock_t*)
 *    | next_free        |  (FreeBlock_t*)
 *    | ...data...       |  block_length bytes
 *    +------------------+
 *
 *  Memory layout (allocated block):
 *    +------------------+  <-- AllocatedBlock_t*
 *    | block_length     |  (size_t)
 *    | ...data...       |  block_length bytes  <-- user pointer
 *    +------------------+
 *
 *  The user pointer = (char*)header + META_DATA_SIZE.
 *  Recovering header  = (char*)userptr - META_DATA_SIZE.
 *
 *  NOTE: sizeof(FreeBlock_t) >= META_DATA_SIZE always.  A block must be at
 *  least sizeof(FreeBlock_t) - META_DATA_SIZE bytes of data to be placed on
 *  the free list (so the list pointers fit inside the data area).
 * ----------------------------------------------------------------------- */
typedef struct FreeBlock_t
{
    size_t              block_length;
    struct FreeBlock_t *prev_free;
    struct FreeBlock_t *next_free;
} FreeBlock_t;

typedef struct
{
    size_t block_length;
} AllocatedBlock_t;

/* Minimum usable data size for a block to live on the free list */
#define MIN_FREE_DATA  (sizeof(FreeBlock_t) - META_DATA_SIZE)

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */
void  Hmm_init(void);
void *HmmAlloc(size_t size);
void  HmmFree(void *ptr);
void *HmmRealloc(void *ptr, size_t size);
void *HmmCalloc(size_t nmemb, size_t size);

/* Debug */
void  Print_FreeList(void);

/* libc overrides */
void *malloc(size_t size);
void  free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

#endif /* _HMM_H_ */
