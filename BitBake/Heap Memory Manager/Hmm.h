#ifndef _HMM_H_
#define _HMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_MEM_ALLOC           (10*1024 + (2*10*sizeof(FreeBlock_t*)) + (10 * sizeof(size_t)))
#define DEFAULT_BLOCK_LENGTH        (1*1024)
#define META_DATA_SIZE              (sizeof(size_t) + (2 * sizeof(FreeBlock_t*)))
#define DEFAULT_LOWERING_VAL        300 * 1024

typedef struct FreeBlock_t
{
    size_t block_length;
    struct FreeBlock_t* prev_free;
    struct FreeBlock_t* next_free;
}FreeBlock_t;

typedef struct
{
    size_t block_length;
}AllocatedBlock_t;

void Hmm_init(void);

void* HmmAlloc(size_t size);
void HmmFree(void* ptr);
void MergeFreeBlocks();
void LowerProgramBreak();

void* HmmRealloc(void *ptr, size_t size);
void* HmmCalloc(size_t nmemb, size_t size);

void Print_FreeList(void);

#endif