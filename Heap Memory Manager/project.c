#include <stdio.h>
#include "Hmm.h"

int main() {
    // Initialize the memory allocator
    Hmm_init();

    // Allocate memory blocks of different sizes
    char *ptr1 = HmmAlloc(1024);
    char *ptr2 = HmmAlloc(1024);
    char *ptr3 = HmmAlloc(1024);

    // Check if allocations were successful
    if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Print the addresses and some data from the allocated memory blocks
    *ptr1= 5;
    *ptr2= 6;
    *ptr3= 7;
    printf("Memory allocated:\n");
    printf("ptr1: Address = %p, Data = %d\n", ptr1, *ptr1);
    printf("ptr2: Address = %p, Data = %d\n", ptr2, *ptr2);
    printf("ptr3: Address = %p, Data = %d\n", ptr3, *ptr3);

    // Free the allocated memory blocks
    HmmFree(ptr1);
    HmmFree(ptr2);
    HmmFree(ptr3);

    return 0;
}
