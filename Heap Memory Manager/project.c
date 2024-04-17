#include <stdio.h>
#include "Hmm.h"

int main() {
    // Initialize the memory allocator
    Hmm_init();

    // Allocate memory blocks of different sizes
    char *ptr1 = (char*)HmmAlloc(1024);
    *ptr1= 5;
    printf("ptr1: Address = %p, Data = %d\n", ptr1, *ptr1);
    
    char *ptr2 = (char*)HmmAlloc(9 * 1024);
    *ptr2= 6;
    printf("ptr2: Address = %p, Data = %d\n", ptr2, *ptr2);

    char *ptr3 = (char*)HmmAlloc(100);
    *ptr3= 7;
    printf("ptr3: Address = %p, Data = %d\n", ptr3, *ptr3);

    char *ptr4 = (char*)HmmAlloc(900);
    *ptr4= 8;
    printf("ptr4: Address = %p, Data = %d\n", ptr4, *ptr4);

    char *ptr5 = (char*)HmmAlloc(1024);
    *ptr5= 9;
    printf("ptr5: Address = %p, Data = %d\n", ptr5, *ptr5);

    char *ptr6 = (char*)HmmAlloc(2000);
    *ptr6= 10;
    printf("ptr6: Address = %p, Data = %d\n", ptr6, *ptr6);


    // Check if allocations were successful
    if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL || ptr4 == NULL || ptr5 == NULL || ptr6 == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Memory allocated Successfully\n");
    

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
