#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "buddy_allocator.h"
#include "malloc_mmap.h"

 
#define MEMORY_SIZE (1 << 20)  // 1MB
#define MIN_BUCKET_SIZE 1     // 1 bytes
#define NUM_LEVELS 21         // log2(MEMORY_SIZE / MIN_BUCKET_SIZE) +1
#define BITMAP_SIZE_BYTES 262144
#define BITMAP_SIZE_BITS 2097151



// small allocations 
void small_buddy_tests() {  
    char* memory = (char*)malloc(MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(BITMAP_SIZE_BYTES);
    BuddyAllocator allocator;
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MEMORY_SIZE , MIN_BUCKET_SIZE);
    
    //test 1
    printf("starting small buddy test num 1\n");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr1 = BuddyAllocator_malloc(&allocator, sizeof(int));
    assert(ptr1 != NULL);
    int* alpha = (int*) ptr1;
    *alpha = 4;
    printf("this should show 4 and istead is shows %d\n",*alpha );
    BuddyAllocator_printBitmap(&allocator);



    //test 2

    printf("starting small buddy test num 2\n");
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
    chad->f=3.5;
    
    assert(chad->first==1);
    assert(chad->second==2);
    assert(chad->third==3);
    assert(chad->c=='a');
    assert(chad->f==3.5);
    BuddyAllocator_printBitmap(&allocator);

    //test 3
    
    printf("starting small buddy test num 3\n");
    void* ptr3 = BuddyAllocator_malloc(&allocator, 50);
    assert(ptr3 != NULL);
    BuddyAllocator_printBitmap(&allocator);

    //test 4
    
    printf("starting small buddy test num 4\n");
    BuddyAllocator_free(&allocator, chad);
    BuddyAllocator_printBitmap(&allocator);

    //test 5
    
    printf("starting small buddy test num 5\n");
    void* ptr4 = BuddyAllocator_malloc(&allocator, 150);
    assert(ptr4 != NULL);
    BuddyAllocator_printBitmap(&allocator);

    //test 6
    printf("starting small buddy test num 6\n");
    BuddyAllocator_free(&allocator, ptr1);
    BuddyAllocator_printBitmap(&allocator);
    
    //test 7
    printf("starting small buddy test num 7\n");
    BuddyAllocator_free(&allocator, ptr3);
    BuddyAllocator_printBitmap(&allocator);
    
    //test 8
    printf("starting small buddy test num 8\n");
    BuddyAllocator_free(&allocator, ptr4);
    BuddyAllocator_printBitmap(&allocator);
    

    printf("all small buddy tests passed.\n");
    

    free(memory);
    free(bitmap_buffer);
}

//chonky allocations
void big_buddy_tests() {
    
    //initialization
    char* memory = (char*)malloc(MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(BITMAP_SIZE_BYTES);
    BuddyAllocator allocator;
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MEMORY_SIZE , MIN_BUCKET_SIZE);
    printf("initialization complete\n");


    //test 1
    printf("starting big boy test num 1\n");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr = BuddyAllocator_malloc(&allocator, MEMORY_SIZE / 2 - sizeof(int));
    assert(ptr != NULL);
    BuddyAllocator_printBitmap(&allocator);
    
    
   // test 2
    printf("starting big boy test num 2\n");
    BuddyAllocator_free(&allocator, ptr);
    BuddyAllocator_printBitmap(&allocator);


    //test 3
    printf("starting big boy test num 3\n");
    void* ptr11 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE/4 - sizeof(int));
    assert(ptr11 != NULL);
    BuddyAllocator_printBitmap(&allocator);

    //test 4
    printf("starting big boy test num 4\n");
    void* ptr4 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE- sizeof(int)); //this shouldn't work because we're out of memory
    assert(ptr4 == NULL);
    BuddyAllocator_printBitmap(&allocator);


    //test 5
    printf("starting big boy test num 5\n");
    BuddyAllocator_free(&allocator, ptr11);
    BuddyAllocator_printBitmap(&allocator);
    
    


    //test 6
    printf("starting big boy test num 6\n");
    void* ptr6 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE- sizeof(int)); //this should work
    assert(ptr6 != NULL);
    BuddyAllocator_printBitmap(&allocator);
    

    //test 7
    printf("starting big boy test num 7\n");
    BuddyAllocator_free(&allocator, ptr6);
    BuddyAllocator_printBitmap(&allocator);



    //test 8
    printf("starting big boy test num 8\n");
    void* ptr7 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE); //this shouldn't work
    assert(ptr7 == NULL);
    BuddyAllocator_printBitmap(&allocator);
    

    printf("Large allocation tests passed.\n");
    free(memory);
    free(bitmap_buffer);
}

void mmap_tests() {

    // test 1
    printf("starting malloc_mmap test num 1\n");
    int size1 = 1024;
    char* ptr1 = (char*)malloc_mmap(size1);
    assert(ptr1 != NULL);
    strcpy(ptr1, "Hello, World!");
    assert(strcmp(ptr1, "Hello, World!") == 0);
    printf("Test 1 passed\n");

    // test 2
    printf("starting malloc_mmap test num 2\n");
    void* ptr2 = malloc_mmap(0);
    assert(ptr2 == NULL);
    printf("Test 2 passed\n");

    // test 3:
    printf("starting malloc_mmap test num 3\n");  
    int size3 = 1024 * 1024 * 10;  // 10 MB
    void* ptr3 = malloc_mmap(size3);
    assert(ptr3 != NULL);
    printf("Test 3 passed\n");

    // test 4
    printf("starting malloc_mmap test num 4\n");
    typedef struct GIGACHAD {
        int first;
        int second;
        int third;
        char c;
        float f;
    }GIGACHAD;

    GIGACHAD* chad = (GIGACHAD*) malloc_mmap(sizeof(GIGACHAD));
    assert(chad != NULL);
    chad->first=1;
    chad->second=2;
    chad->third=3;
    chad->c='a';
    chad->f=3.5;
    assert(chad->first==1);
    assert(chad->second==2);
    assert(chad->third==3);
    assert(chad->c=='a');
    assert(chad->f==3.5);
    printf("Test 4 passed\n");

    //test 5
    free_mmap(ptr1);
    printf("Test 5 passed\n");
    
    //test 6
    free_mmap(ptr2);
    printf("Test 6 passed\n");

    //test 7
    free_mmap(ptr3);
    printf("Test 7 passed\n");
    
    //test 8
    free_mmap(chad);
    printf("Test 8 passed\n");

    printf("malloc_mmap allocation test passed.\n");
}


int main() {
    small_buddy_tests();
    big_buddy_tests();
    mmap_tests();
    printf("All tests passed successfully!\n");
    return 0;
}