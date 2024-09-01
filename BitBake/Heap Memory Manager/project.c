#include <stdio.h>
#include "Hmm.h"

int main() {
    // Initialize the memory allocator
    Hmm_init();

    // Allocate memory blocks of different sizes
    char *ptr1 = (char*)HmmAlloc(1024);
    
    //printf("ptr1: Address = %p, Data = %d\n", ptr1, *ptr1);
    
    char *ptr2 = (char*)HmmAlloc(9 * 1024);
    
    //printf("ptr2: Address = %p, Data = %d\n", ptr2, *ptr2);

    char *ptr3 = (char*)HmmAlloc(100);

    //printf("ptr3: Address = %p, Data = %d\n", ptr3, *ptr3);

    char *ptr4 = (char*)HmmAlloc(900);
    
    //printf("ptr4: Address = %p, Data = %d\n", ptr4, *ptr4);

    char *ptr5 = (char*)HmmAlloc(1024);
    
    //printf("ptr5: Address = %p, Data = %d\n", ptr5, *ptr5);

    char *ptr6 = (char*)HmmAlloc(2000);
    
    //printf("ptr6: Address = %p, Data = %d\n", ptr6, *ptr6);


    *ptr1= 1;
    *ptr2= 6;
    *ptr3= 7;
    *ptr4= 8;
    *ptr5= 9;
    *ptr6= 10;
    

    // Free the allocated memory blocks
    HmmFree(ptr1);
    HmmFree(ptr1);
    HmmFree(ptr2);
    HmmFree(ptr3);
    HmmFree(ptr4);
    HmmFree(ptr4);
    HmmFree(ptr5);
    HmmFree(ptr6);

    return 0;
}
