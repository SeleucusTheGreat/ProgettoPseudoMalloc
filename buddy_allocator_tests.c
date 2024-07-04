#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "buddy_allocator.h"

 
#define MEMORY_SIZE (1 << 20)  // 1MB
#define MIN_BUCKET_SIZE 1     // 1 bytes
#define NUM_LEVELS 21         // log2(MEMORY_SIZE / MIN_BUCKET_SIZE) +1
#define BITMAP_SIZE_BYTES 262144
#define BITMAP_SIZE_BITS 2097151


// small allocations 
void test_allocation_and_deallocation() {  
    char* memory = (char*)malloc(MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(BITMAP_SIZE_BYTES);
    BuddyAllocator allocator;
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MEMORY_SIZE , MIN_BUCKET_SIZE);
    
    //test 0
    printf("situation before the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr1 = BuddyAllocator_malloc(&allocator, sizeof(int));
    assert(ptr1 != NULL);
    int* alpha = (int*) ptr1;
    *alpha = 4;
    printf("this should show 4 and istead is shows %d\n",*alpha );
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);



    //test 2

    typedef struct GIGACHAD {
        int first;
        int second;
        int third;
        char c;
        float f;
    }GIGACHAD;
    
    GIGACHAD* chad = (GIGACHAD*) BuddyAllocator_malloc(&allocator, sizeof(GIGACHAD));
    assert(chad != NULL);
    chad->first=1;
    chad->second=2;
    chad->third=3;
    chad->c='a';
    chad->f=3.223;
    
    printf("this should show 1,2,3,a,3.223 and instead it is showing %d,%d,%d,%c,%.3f\n", chad->first, chad->second, chad->third, chad->c, chad->f);

    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 3
    
    void* ptr3 = BuddyAllocator_malloc(&allocator, 50);
    assert(ptr3 != NULL);

    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 4
    
    BuddyAllocator_free(&allocator, chad);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);

    //test 5
    
    void* ptr4 = BuddyAllocator_malloc(&allocator, 150);
    assert(ptr4 != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 6
    
    BuddyAllocator_free(&allocator, ptr1);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    
    //test 7
   
    BuddyAllocator_free(&allocator, ptr3);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    
    //test 8
    
    BuddyAllocator_free(&allocator, ptr4);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    
    //final check
    printf("Allocation and deallocation test passed.\n");
    

    free(memory);
    free(bitmap_buffer);
}

//chonky allocations
void test_large_allocation() {
    char* memory = (char*)malloc(MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(BITMAP_SIZE_BYTES);
    BuddyAllocator allocator;
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MEMORY_SIZE , MIN_BUCKET_SIZE);
    
    //test 9
    printf("situation before  the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr = BuddyAllocator_malloc(&allocator, MEMORY_SIZE / 2 - sizeof(int));
    assert(ptr != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);
    
    
   // test 10
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
    BuddyAllocator_free(&allocator, ptr);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);


    //test 11
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr11 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE/4 - sizeof(int));
    assert(ptr11 != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);


    //test 12
    BuddyAllocator_free(&allocator, ptr11);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    printf("Large allocation test passed.\n");


    //test 13
    void* ptr13 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE- sizeof(int)); //this should work
    assert(ptr13 != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);
    
    BuddyAllocator_free(&allocator, ptr13);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    printf("Large allocation test passed.\n");


    //test 14
    void* ptr14 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE); //this shouldn't work
    assert(ptr14 != NULL);
    
    free(memory);
    free(bitmap_buffer);
}

int main() {
    test_allocation_and_deallocation();
    test_large_allocation();
    
    printf("All tests passed successfully!\n");
    return 0;
}