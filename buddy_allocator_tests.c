#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "buddy_allocator.h"

 
#define MEMORY_SIZE (1 << 20)  // 1MB
#define MIN_BUCKET_SIZE 1     // 1 bytes
#define NUM_LEVELS 21         // log2(MEMORY_SIZE / MIN_BUCKET_SIZE) +1
#define BITMAP_SIZE_BYTES 262144
#define BITMAP_SIZE_BITS 2097152


// small allocations 
void test_allocation_and_deallocation() {  
    char* memory = (char*)malloc(MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(BITMAP_SIZE_BYTES);
    BuddyAllocator allocator;
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MIN_BUCKET_SIZE);
    
    //test 1
    printf("situation before the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr1 = BuddyAllocator_malloc(&allocator, 100);
    assert(ptr1 != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 2
    printf("situation before the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr2 = BuddyAllocator_malloc(&allocator, 200);
    assert(ptr2 != NULL);
    assert(ptr2 != ptr1);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 3
    printf("situation before  the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr3 = BuddyAllocator_malloc(&allocator, 50);
    assert(ptr3 != NULL);
    assert(ptr3 != ptr1 && ptr3 != ptr2);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 4
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
    BuddyAllocator_free(&allocator, ptr2);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);

    //test 5
    printf("situation before  the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr4 = BuddyAllocator_malloc(&allocator, 150);
    assert(ptr4 != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);

    //test 6
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
    BuddyAllocator_free(&allocator, ptr1);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    
    //test 7
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
    BuddyAllocator_free(&allocator, ptr3);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);
    
    //test 8
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
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
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MIN_BUCKET_SIZE);
    
    //test 9
    printf("situation before  the malloc");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr = BuddyAllocator_malloc(&allocator, MEMORY_SIZE / 2);
    assert(ptr != NULL);
    printf("situation after the malloc");
    BuddyAllocator_printBitmap(&allocator);
    
    
    //test 10
    printf("situation before  the free");
    BuddyAllocator_printBitmap(&allocator);
    BuddyAllocator_free(&allocator, ptr);
    printf("situation after the free");
    BuddyAllocator_printBitmap(&allocator);



    printf("Large allocation test passed.\n");
    
    free(memory);
    free(bitmap_buffer);
}

int main() {
    test_allocation_and_deallocation();
    test_large_allocation();
    
    printf("All tests passed successfully!\n");
    return 0;
}