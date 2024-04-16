#include <stdio.h>
#include "Hmm.h"

int main() {
    // Initialize the memory allocator
    Hmm_init();

    // Allocate memory blocks of different sizes
    char *ptr1 = (char*)HmmAlloc(1024);
    
    char *ptr2 = (char*)HmmAlloc(1024);
    char *ptr3 = (char*)HmmAlloc(100);
    char *ptr4 = (char*)HmmAlloc(900);
    char *ptr5 = (char*)HmmAlloc(1024);
    char *ptr6 = (char*)HmmAlloc(2000);


    // Check if allocations were successful
    if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Print the addresses and some data from the allocated memory blocks
    *ptr1= 5;
    *ptr2= 6;
    *ptr3= 7;
    *ptr4= 8;
    *ptr5= 9;
    *ptr6= 10;
    printf("Memory allocated:\n");
    printf("ptr1: Address = %p, Data = %d\n", ptr1, *ptr1);
    printf("ptr2: Address = %p, Data = %d\n", ptr2, *ptr2);
    printf("ptr3: Address = %p, Data = %d\n", ptr3, *ptr3);
    printf("ptr4: Address = %p, Data = %d\n", ptr4, *ptr4);
    printf("ptr5: Address = %p, Data = %d\n", ptr5, *ptr5);
    printf("ptr6: Address = %p, Data = %d\n", ptr6, *ptr6);

    // Free the allocated memory blocks
    HmmFree(ptr1);
    HmmFree(ptr1);
    HmmFree(ptr2);
    HmmFree(ptr3);
    HmmFree(ptr4);
    HmmFree(ptr5);
    HmmFree(ptr6);

    return 0;
}
