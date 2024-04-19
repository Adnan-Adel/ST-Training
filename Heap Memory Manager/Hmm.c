#include "Hmm.h"

static FreeBlock_t* first_free= NULL;
static FreeBlock_t* last_free= NULL;

void Hmm_init(void)
{
    // Allocate initial memory block using sbrk
    first_free= (FreeBlock_t*)sbrk(DEFAULT_MEM_ALLOC);


    // Calculate the size of each block including metadata
    size_t block_size = sizeof(size_t) + 2 * sizeof(FreeBlock_t*) + DEFAULT_BLOCK_LENGTH;
    
    // Initialize first free block
    first_free->block_length= DEFAULT_BLOCK_LENGTH;
    first_free->prev_free= NULL;
    first_free->next_free= NULL;

    // Traverse Allocated Memory reigion and make 10 blocks of data size= 1Kbytes
    FreeBlock_t* current_free_block = first_free;
    size_t temp_length= (DEFAULT_BLOCK_LENGTH + META_DATA_SIZE);
    while(temp_length < DEFAULT_MEM_ALLOC)
    {
        FreeBlock_t* next_free_block = (FreeBlock_t*)((char*)current_free_block + block_size);
        next_free_block->block_length = DEFAULT_BLOCK_LENGTH;
        next_free_block->prev_free = current_free_block;
        next_free_block->next_free = NULL;

        temp_length += (DEFAULT_BLOCK_LENGTH + META_DATA_SIZE);
        current_free_block->next_free = next_free_block;
        current_free_block = next_free_block;
    }

    /*Debugging*/

    /*printf("Initialized memory allocator with 10 blocks of size 1KB each.\n");
    printf("Memory layout:\n");
    current_free_block = first_free;
    int block_number = 1;
    while (current_free_block != NULL)
    {
        printf("Block %d:\n", block_number++);
        printf("    Address: %p\n", current_free_block);
        printf("    Block Length: %zu bytes\n", current_free_block->block_length);
        printf("    Previous Free Block: %p\n", current_free_block->prev_free);
        printf("    Next Free Block: %p\n", current_free_block->next_free);
        printf("    Data Area Address: %p\n", (char*)current_free_block + sizeof(FreeBlock_t));
        printf("\n");

        current_free_block = current_free_block->next_free;
    }

    // Print data in each block's data area
    printf("Data in Data Area:\n");
    current_free_block = first_free;
    while (current_free_block != NULL)
    {
        char* data_area = (char*)current_free_block + sizeof(FreeBlock_t);
        for (int i = 0; i < DEFAULT_BLOCK_LENGTH; ++i) 
        {
            printf("%d", i);  // Assign 'A' to 'Z' cyclically
        }
        printf("\n\n");
        current_free_block = current_free_block->next_free;
    }
    printf("\n\n");*/
    
}



void* HmmAlloc(size_t size)
{
    FreeBlock_t* current_node= first_free;
    FreeBlock_t* prev_node= NULL;
    char* AllocMem= NULL;

    while(current_node != NULL)
    {
        if(size == current_node->block_length)
        {
            // Remove Allocated Block from Free List
            if(prev_node == NULL)
            {
                // First Node in List, SO move first free
                first_free= first_free->next_free;
                if(first_free != NULL)
                {
                    first_free->prev_free= NULL;
                }
            }
            else
            {
                prev_node->next_free= current_node->next_free;
                if(current_node->next_free != NULL)
                {
                    current_node->next_free->prev_free= prev_node;
                }
            }

            current_node->prev_free= NULL;
            current_node->next_free= NULL;

            // Update AllocBlock to point to the same memory location as current_node
            AllocatedBlock_t* AllocBlock= (AllocatedBlock_t*)current_node;
            AllocBlock->block_length= current_node->block_length;
            
            AllocMem= (void*)((char*)AllocBlock + sizeof(FreeBlock_t));

            return AllocMem;
        }
        else if(size < current_node->block_length)
        {
            size_t TotalNewBlock_size= size + sizeof(FreeBlock_t);
            size_t FreeData_size= current_node->block_length;
            
            if(TotalNewBlock_size < FreeData_size)
            {   // Split Block into two blocks, one of them would be of suitable size
                // Make a new block in free data area of current block
                current_node->block_length= current_node->block_length - TotalNewBlock_size;
                
                FreeBlock_t* TempFreeBlock= (FreeBlock_t*)((char*)current_node + sizeof(FreeBlock_t) + current_node->block_length);
                TempFreeBlock->block_length= size;
                TempFreeBlock->prev_free= current_node;
                TempFreeBlock->next_free= current_node->next_free;

                current_node->next_free= TempFreeBlock;

                prev_node= current_node;
                current_node= current_node->next_free;

                // Remove Allocated Block from Free List
                prev_node->next_free= current_node->next_free;
                if(current_node->next_free != NULL)
                {
                    current_node->next_free->prev_free= prev_node;
                }

                current_node->prev_free= NULL;
                current_node->next_free= NULL;

                // Update AllocBlock to point to the same memory location as current_node
                AllocatedBlock_t* AllocBlock= (AllocatedBlock_t*)current_node;
                AllocBlock->block_length= current_node->block_length;
                
                AllocMem= (void*)((char*)AllocBlock + sizeof(FreeBlock_t));

                return AllocMem;
            }
            else
            {
                // Remove Allocated Block from Free List
                if(prev_node == NULL)
                {
                    // First Node in List, SO move first free
                    first_free= first_free->next_free;
                    if(first_free != NULL)
                    {
                        first_free->prev_free= NULL;
                    }
                }
                else
                {
                    prev_node->next_free= current_node->next_free;
                    if(current_node->next_free != NULL)
                    {
                        current_node->next_free->prev_free= prev_node;
                    }
                }

                current_node->prev_free= NULL;
                current_node->next_free= NULL;

                // Update AllocBlock to point to the same memory location as current_node
                AllocatedBlock_t* AllocBlock= (AllocatedBlock_t*)current_node;
                AllocBlock->block_length= current_node->block_length;
                
                AllocMem= (void*)((char*)AllocBlock + sizeof(FreeBlock_t));

                return AllocMem;
            }
        }
        else if(size > current_node->block_length)
        {
            FreeBlock_t* TempCurrentFree= (FreeBlock_t*)current_node;
            FreeBlock_t* TempNextFree= (FreeBlock_t*)current_node->next_free;
            size_t sum_length= current_node->block_length;      // Size of data in current block
            unsigned int num_of_blocks= 0;
            while(sum_length < size)
            {
                if(TempNextFree == NULL)
                {
                    break;
                }
                else if(((char*)TempCurrentFree + sum_length + sizeof(FreeBlock_t)) == ((char*)TempNextFree))
                {
                    sum_length += TempNextFree->block_length + META_DATA_SIZE;
                    num_of_blocks++;
                }
                else
                {
                    // Not Contiguous
                    TempCurrentFree= TempNextFree;
                    sum_length= TempCurrentFree->block_length;
                    num_of_blocks= 0;
                }
                TempNextFree= TempNextFree->next_free;
            }
            current_node= TempCurrentFree;
            
            if(sum_length >= size)
            {
                // Merge those free blocks
                FreeBlock_t* NextFreeBlock= current_node->next_free;
                while((NextFreeBlock != NULL) && (num_of_blocks--))
                {
                    // Check if current free block and next free block are contiguous
                    if(((char*)current_node + current_node->block_length + sizeof(FreeBlock_t)) == ((char*)NextFreeBlock))
                    {
                        // Merge two blocks
                        current_node->block_length= current_node->block_length + NextFreeBlock->block_length + sizeof(FreeBlock_t);
                        current_node->next_free= NextFreeBlock->next_free;
                    }
                    else
                    {
                        break;
                    }
                    // Update Next Free
                    NextFreeBlock= NextFreeBlock->next_free;
                    if(NextFreeBlock != NULL)
                    {
                        NextFreeBlock->prev_free= current_node;
                    }
                }

                if(current_node->prev_free == NULL)
                {
                    first_free= current_node->next_free;
                }
                else
                {
                    current_node->prev_free->next_free= current_node->next_free;
                }
                
                if(current_node->next_free != NULL)
                {
                    current_node->next_free->prev_free= current_node->prev_free;
                }

                current_node->next_free= NULL;
                current_node->prev_free= NULL;
                
                // Allocate the merged bigger block for user
                AllocatedBlock_t* AllocBlock= (AllocatedBlock_t*)current_node;
                AllocBlock->block_length= current_node->block_length;
                
                AllocMem= (void*)((char*)AllocBlock + sizeof(FreeBlock_t));

                return AllocMem; 
            }

            // Move to next node
            prev_node= current_node;
            current_node= current_node->next_free;
        }
    }

    // No suitable block in free list for the required block, allocate new memory using sbrk
    size_t block_size = size + META_DATA_SIZE; // Include metadata size
    size_t Alloc_size= block_size * 10;

    current_node= (FreeBlock_t*)sbrk(Alloc_size);
    current_node->block_length= size;
    if(first_free == NULL)
    {
        current_node->prev_free= NULL;
        first_free= current_node;
    }
    else
    {
        // Find the last block in the free list and connect it with the free block
        FreeBlock_t* last_free = first_free;
        while (last_free->next_free != NULL)
        {
            last_free = last_free->next_free;
        }
        last_free->next_free = current_node;
        current_node->prev_free = last_free;
    }

    // Split this new area into 10 bloks of block size
    FreeBlock_t* temp_current_node = current_node;
    size_t temp_length= (block_size);
    while(temp_length < Alloc_size)
    {
        FreeBlock_t* next_free_block = (FreeBlock_t*)((char*)temp_current_node + block_size);
        next_free_block->block_length = size;
        next_free_block->prev_free = temp_current_node;
        next_free_block->next_free = NULL;

        temp_length += temp_current_node->block_length + META_DATA_SIZE;
        temp_current_node->next_free = next_free_block;
        temp_current_node = next_free_block;
    }


    // Allocate current block for user
    if(current_node->prev_free == NULL)
    {
        first_free= current_node->next_free;
    }
    else
    {
        current_node->prev_free->next_free= current_node->next_free;
    }
    
    if(current_node->next_free != NULL)
    {
        current_node->next_free->prev_free= current_node->prev_free;
    }

    current_node->next_free= NULL;
    current_node->prev_free= NULL;
    
    // Allocate the merged bigger block for user
    AllocatedBlock_t* AllocBlock= (AllocatedBlock_t*)current_node;
    AllocBlock->block_length= current_node->block_length;
    
    AllocMem= (void*)((char*)AllocBlock + sizeof(FreeBlock_t));

    return AllocMem;

}

void HmmFree(void* ptr)
{
    
    if(ptr == NULL)
    {
        printf("Attempting to free NULL Pointer!!!!\n");
        return;
    }

    // Find the corresponding start of allocated block
    FreeBlock_t* block_start = (FreeBlock_t*)((char*)ptr - sizeof(FreeBlock_t));

    // Check if block has already been freed before
    FreeBlock_t* current_free= first_free;
    while(current_free != NULL)
    {
        if((block_start == current_free))
        {
            printf("Block has already been freed before!!!\n");
            return;
        }
        else if((block_start > current_free) && (block_start < current_free->next_free))
        {
            FreeBlock_t* next= current_free + current_free->block_length + 2*META_DATA_SIZE + block_start->block_length;
            if(current_free->next_free == next)
            {
                printf("Block has already been freed before!!!\n");
                return;
            }
        }
        current_free= current_free->next_free;
    }

    // Add the block back to the free list
    if (first_free == NULL)
    {
        // If the free list is empty, set the free block as the first block
        first_free = block_start;
    }
    
    else
    {
        // Find the last block in the free list and connect it with the free block
        FreeBlock_t* last_free = first_free;
        while (last_free->next_free != NULL)
        {
            last_free = last_free->next_free;
        }
        last_free->next_free = block_start;
        block_start->prev_free = last_free;
    }

    block_start->next_free= NULL;

    // Merge Adjacent Free Blocks
    //MergeFreeBlocks();
}

void MergeFreeBlocks(void)
{
    FreeBlock_t* CurrentFreeBlock= first_free;
    FreeBlock_t* NextFreeBlock= CurrentFreeBlock;
    while(CurrentFreeBlock != NULL)
    {
        while(NextFreeBlock != NULL)
        {
            NextFreeBlock= CurrentFreeBlock->next_free;
            // Check if current free block and next free block are contiguous
            if(((char*)CurrentFreeBlock + CurrentFreeBlock->block_length + sizeof(FreeBlock_t)) == ((char*)NextFreeBlock))
            {
                // Merge two blocks
                CurrentFreeBlock->block_length= CurrentFreeBlock->block_length + NextFreeBlock->block_length + sizeof(FreeBlock_t);
                CurrentFreeBlock->next_free= NextFreeBlock->next_free;
            }
            else
            {
                break;
            }
            // Update Next Free
            NextFreeBlock= NextFreeBlock->next_free;
            if(NextFreeBlock != NULL)
            {
                NextFreeBlock->prev_free= CurrentFreeBlock;
            }
        }
        CurrentFreeBlock= CurrentFreeBlock->next_free;
    }
}

