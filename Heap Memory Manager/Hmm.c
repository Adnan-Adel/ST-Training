/*
 * Hmm.c  –  Heap Memory Manager
 *
 * Design
 * ------
 *  - Doubly-linked free list, kept in ascending address order so that
 *    adjacent-block detection is O(1) per insertion.
 *  - First-fit allocation with block splitting.
 *  - Coalescing (merging) on every HmmFree call.
 *  - Program-break lowering when the topmost free block is large enough.
 *  - sbrk() used to grow the heap; new memory is carved into a chain of
 *    DEFAULT_BLOCK_LENGTH blocks and appended to the free list.
 */

#include "Hmm.h"

/* -----------------------------------------------------------------------
 * Module-private state
 * ----------------------------------------------------------------------- */
static FreeBlock_t *first_free = NULL;

/* -----------------------------------------------------------------------
 * Internal helper prototypes
 * ----------------------------------------------------------------------- */
static void         freelist_insert_sorted(FreeBlock_t *block);
static void         freelist_remove(FreeBlock_t *block);
static void        *allocate_block(FreeBlock_t *block, size_t size);
static FreeBlock_t *split_block(FreeBlock_t *block, size_t size);
static void         merge_adjacent(FreeBlock_t *block);
static FreeBlock_t *grow_heap(size_t size);
static size_t       align8(size_t size);
static void         LowerProgramBreak(void);

/* -----------------------------------------------------------------------
 * align8
 *
 * Round size up to the nearest multiple of 8 bytes.
 * ----------------------------------------------------------------------- */
static size_t align8(size_t size)
{
    return (size + 7u) & ~7u;
}

/* -----------------------------------------------------------------------
 * freelist_insert_sorted
 *
 * Insert a free block into the free list, maintaining ascending address
 * order.  Keeping the list sorted is what makes O(1) neighbour detection
 * possible in merge_adjacent().
 * ----------------------------------------------------------------------- */
static void freelist_insert_sorted(FreeBlock_t *block)
{
    block->prev_free = NULL;
    block->next_free = NULL;

    if (first_free == NULL)
    {
        first_free = block;
        return;
    }

    /* Find the first list node that lies at a higher address than block */
    FreeBlock_t *cur = first_free;
    while (cur != NULL && cur < block)
    {
        cur = cur->next_free;
    }

    if (cur == NULL)
    {
        /* block goes at the tail */
        FreeBlock_t *tail = first_free;
        while (tail->next_free != NULL)
            tail = tail->next_free;

        tail->next_free  = block;
        block->prev_free = tail;
    }
    else if (cur == first_free)
    {
        /* block goes at the head */
        block->next_free     = first_free;
        first_free->prev_free = block;
        first_free           = block;
    }
    else
    {
        /* block goes between cur->prev_free and cur */
        FreeBlock_t *prev = cur->prev_free;
        prev->next_free  = block;
        block->prev_free = prev;
        block->next_free = cur;
        cur->prev_free   = block;
    }
}

/* -----------------------------------------------------------------------
 * freelist_remove
 *
 * Unlink a block from the free list without touching block_length.
 * ----------------------------------------------------------------------- */
static void freelist_remove(FreeBlock_t *block)
{
    if (block->prev_free != NULL)
        block->prev_free->next_free = block->next_free;
    else
        first_free = block->next_free;          /* block was the head */

    if (block->next_free != NULL)
        block->next_free->prev_free = block->prev_free;

    block->prev_free = NULL;
    block->next_free = NULL;
}

/* -----------------------------------------------------------------------
 * allocate_block
 *
 * Remove a free block from the list and return the user data pointer.
 * The caller must already have removed (or arranged to remove) the block;
 * this function only sets up the AllocatedBlock_t header and returns the
 * data pointer.
 * ----------------------------------------------------------------------- */
static void *allocate_block(FreeBlock_t *block, size_t size)
{
    freelist_remove(block);

    /* Reinterpret the same memory as an allocated block header */
    AllocatedBlock_t *hdr = (AllocatedBlock_t *)block;
    hdr->block_length     = size;               /* record the requested size */

    return (char *)hdr + META_DATA_SIZE;
}

/* -----------------------------------------------------------------------
 * split_block
 *
 * Given a free block that is larger than needed, carve off a piece of
 * exactly `size` bytes from the HIGH end, leave the remainder on the free
 * list, and return a pointer to the carved-off piece (still a FreeBlock_t*
 * so the caller can call allocate_block on it).
 *
 * We split from the high end so the original block keeps its list position
 * and pointers need no update.
 *
 * Returns NULL if there is not enough room to split (remainder would be
 * too small to hold FreeBlock_t metadata).
 * ----------------------------------------------------------------------- */
static FreeBlock_t *split_block(FreeBlock_t *block, size_t size)
{
    size_t needed_total = size + META_DATA_SIZE; /* header + data for new block */

    /* The remainder must be large enough to store the FreeBlock_t header */
    if (block->block_length < needed_total + MIN_FREE_DATA)
        return NULL;                             /* can't split cleanly */

    /* Shrink the existing free block */
    block->block_length -= needed_total;

    /* The new (allocated) block starts right after the shrunk free block */
    FreeBlock_t *new_block = (FreeBlock_t *)((char *)block
                                              + META_DATA_SIZE
                                              + block->block_length);
    new_block->block_length = size;
    new_block->prev_free    = NULL;
    new_block->next_free    = NULL;

    return new_block;
}

/* -----------------------------------------------------------------------
 * merge_adjacent
 *
 * Coalesce `block` with its immediate neighbours in the free list if they
 * are physically contiguous in memory.  Because the free list is kept in
 * sorted address order, checking neighbours is O(1).
 *
 * Call this AFTER inserting `block` into the free list.
 * ----------------------------------------------------------------------- */
static void merge_adjacent(FreeBlock_t *block)
{
    /* Try to merge with the NEXT block */
    FreeBlock_t *next = block->next_free;
    if (next != NULL)
    {
        char *expected_next = (char *)block + META_DATA_SIZE + block->block_length;
        if (expected_next == (char *)next)
        {
            block->block_length += META_DATA_SIZE + next->block_length;
            /* unlink next */
            block->next_free = next->next_free;
            if (next->next_free != NULL)
                next->next_free->prev_free = block;
        }
    }

    /* Try to merge with the PREVIOUS block */
    FreeBlock_t *prev = block->prev_free;
    if (prev != NULL)
    {
        char *expected_block = (char *)prev + META_DATA_SIZE + prev->block_length;
        if (expected_block == (char *)block)
        {
            prev->block_length += META_DATA_SIZE + block->block_length;
            /* unlink block */
            prev->next_free = block->next_free;
            if (block->next_free != NULL)
                block->next_free->prev_free = prev;
        }
    }
}

/* -----------------------------------------------------------------------
 * grow_heap
 *
 * Ask the OS for more memory via sbrk(), carve it into blocks of `size`
 * bytes each, add them all to the free list, and return the first block
 * (which the caller will immediately allocate).
 * ----------------------------------------------------------------------- */
static FreeBlock_t *grow_heap(size_t size)
{
    size_t block_total = META_DATA_SIZE + size;  /* one block, header + data */
    size_t alloc_total = block_total * 10;       /* ask for 10 blocks at once */

    FreeBlock_t *region = (FreeBlock_t *)sbrk((intptr_t)alloc_total);
    if (region == (FreeBlock_t *)-1)
        return NULL;                             /* sbrk failed */

    /* Carve the region into 10 blocks and add each to the sorted free list */
    char *cursor = (char *)region;
    char *end    = cursor + alloc_total;

    while (cursor + block_total <= end)
    {
        FreeBlock_t *blk  = (FreeBlock_t *)cursor;
        blk->block_length = size;
        blk->prev_free    = NULL;
        blk->next_free    = NULL;
        freelist_insert_sorted(blk);
        cursor += block_total;
    }

    /* Return (and implicitly promise to allocate) the first block */
    return region;
}

/* -----------------------------------------------------------------------
 * Hmm_init
 *
 * Pre-populate the free list with DEFAULT_MEM_ALLOC bytes from sbrk(),
 * split into blocks of DEFAULT_BLOCK_LENGTH bytes each.
 * ----------------------------------------------------------------------- */
void Hmm_init(void)
{
    void *region = sbrk((intptr_t)DEFAULT_MEM_ALLOC);
    if (region == (void *)-1)
        return;

    size_t block_total = META_DATA_SIZE + DEFAULT_BLOCK_LENGTH;
    char  *cursor      = (char *)region;
    char  *end         = cursor + DEFAULT_MEM_ALLOC;

    while (cursor + block_total <= end)
    {
        FreeBlock_t *blk  = (FreeBlock_t *)cursor;
        blk->block_length = DEFAULT_BLOCK_LENGTH;
        blk->prev_free    = NULL;
        blk->next_free    = NULL;
        freelist_insert_sorted(blk);
        cursor += block_total;
    }
}

/* -----------------------------------------------------------------------
 * HmmAlloc
 *
 * First-fit allocator.
 *   1. Walk the free list for the first block >= size.
 *   2. If the block is larger, try to split it.
 *   3. If no suitable block exists, grow the heap.
 * ----------------------------------------------------------------------- */
void *HmmAlloc(size_t size)
{
    if (size == 0)
        return NULL;

    size = align8(size);

    FreeBlock_t *cur = first_free;

    while (cur != NULL)
    {
        if (cur->block_length >= size)
        {
            /* Found a candidate.  Try to split first. */
            FreeBlock_t *piece = split_block(cur, size);
            if (piece != NULL)
            {
                /* piece is a freshly carved-off block, not yet on the list */
                return allocate_block(piece, size);
            }
            else
            {
                /* Can't split cleanly – give the whole block */
                return allocate_block(cur, cur->block_length);
            }
        }
        cur = cur->next_free;
    }

    /* No suitable block found – grow the heap */
    FreeBlock_t *new_block = grow_heap(size);
    if (new_block == NULL)
        return NULL;

    FreeBlock_t *piece = split_block(new_block, size);
    if (piece != NULL)
        return allocate_block(piece, size);
    else
        return allocate_block(new_block, new_block->block_length);
}

/* -----------------------------------------------------------------------
 * HmmFree
 *
 *  1. Validate pointer.
 *  2. Insert the block back into the free list (sorted by address).
 *  3. Coalesce with neighbours.
 *  4. Optionally lower the program break.
 * ----------------------------------------------------------------------- */
void HmmFree(void *ptr)
{
    if (ptr == NULL)
        return;

    FreeBlock_t *block = (FreeBlock_t *)((char *)ptr - META_DATA_SIZE);

    /* Guard: check the block is not already on the free list (double-free) */
    for (FreeBlock_t *cur = first_free; cur != NULL; cur = cur->next_free)
    {
        if (cur == block)
            return;                              /* already free – ignore */
    }

    freelist_insert_sorted(block);
    merge_adjacent(block);
    LowerProgramBreak();
}

/* -----------------------------------------------------------------------
 * LowerProgramBreak
 *
 * If the free block at the highest address is large enough, release it
 * back to the OS via sbrk(-n).
 * ----------------------------------------------------------------------- */
void LowerProgramBreak(void)
{
    if (first_free == NULL)
        return;

    /* Find the free block with the highest address */
    FreeBlock_t *highest = first_free;
    for (FreeBlock_t *cur = first_free->next_free; cur != NULL; cur = cur->next_free)
    {
        if (cur > highest)
            highest = cur;
    }

    size_t total_size = META_DATA_SIZE + highest->block_length;
    if (total_size < DEFAULT_LOWERING_VAL)
        return;

    /* Verify this block is truly at the top of the heap */
    char *block_end = (char *)highest + total_size;
    if (block_end != (char *)sbrk(0))
        return;                                  /* not at the program break */

    freelist_remove(highest);
    sbrk(-(intptr_t)total_size);
}

/* -----------------------------------------------------------------------
 * HmmRealloc
 * ----------------------------------------------------------------------- */
void *HmmRealloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return HmmAlloc(size);

    if (size == 0)
    {
        HmmFree(ptr);
        return NULL;
    }

    size = align8(size);

    AllocatedBlock_t *hdr      = (AllocatedBlock_t *)((char *)ptr - META_DATA_SIZE);
    size_t            old_size = hdr->block_length;

    if (size <= old_size)
        return ptr;                              /* shrink-in-place */

    void *new_ptr = HmmAlloc(size);
    if (new_ptr == NULL)
        return NULL;

    memcpy(new_ptr, ptr, old_size);
    HmmFree(ptr);
    return new_ptr;
}

/* -----------------------------------------------------------------------
 * HmmCalloc
 * ----------------------------------------------------------------------- */
void *HmmCalloc(size_t nmemb, size_t size)
{
    /* Guard against overflow in nmemb * size */
    if (nmemb != 0 && size > (size_t)-1 / nmemb)
        return NULL;

    size_t total = nmemb * size;
    void  *ptr   = HmmAlloc(total);
    if (ptr != NULL)
        memset(ptr, 0, total);
    return ptr;
}

/* -----------------------------------------------------------------------
 * Print_FreeList  (debug)
 * ----------------------------------------------------------------------- */
void Print_FreeList(void)
{
    printf("=== Free List ===\n");
    FreeBlock_t *cur = first_free;
    int i = 0;
    while (cur != NULL)
    {
        printf("[%d] addr=%p  len=%zu  prev=%p  next=%p\n",
               i, (void *)cur, cur->block_length,
               (void *)cur->prev_free, (void *)cur->next_free);
        cur = cur->next_free;
        i++;
    }
    printf("=================\n");
}

/* -----------------------------------------------------------------------
 * libc overrides
 * ----------------------------------------------------------------------- */
void *malloc(size_t size)               { return HmmAlloc(size); }
void  free(void *ptr)                   { HmmFree(ptr); }
void *calloc(size_t nmemb, size_t size) { return HmmCalloc(nmemb, size); }
void *realloc(void *ptr, size_t size)   { return HmmRealloc(ptr, size); }
