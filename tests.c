#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "buddy_allocator.h"
#include "malloc_mmap.h"
#include "pseudoMalloc.h"

 
#define MEMORY_SIZE (1 << 20)  // 1MB
#define MIN_BUCKET_SIZE 1     // 1 bytes
#define NUM_LEVELS 21         // log2(MEMORY_SIZE / MIN_BUCKET_SIZE) +1
#define BITMAP_SIZE_BYTES 262144
#define BITMAP_SIZE_BITS 2097151

void small_buddy_tests();
void big_buddy_tests();
void mmap_tests();
void pseudoMalloc_tests();

// small allocations 
void small_buddy_tests() {  

    printf("\n\n\n"); 
    printf("-----------starting buddyAllocator tests-----------\n");
    char* memory = (char*)malloc(MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(BITMAP_SIZE_BYTES);
    BuddyAllocator allocator;
    BuddyAllocator_init(&allocator, NUM_LEVELS, bitmap_buffer, BITMAP_SIZE_BYTES , memory, MEMORY_SIZE , MIN_BUCKET_SIZE);
    
    //test 1
    printf("\n\n");
    printf("----starting small buddy test num 1----\n");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr1 = BuddyAllocator_malloc(&allocator, sizeof(int));
    assert(ptr1 != NULL);
    int* alpha = (int*) ptr1;
    *alpha = 4;
    printf("this should show 4 and istead is shows %d\n",*alpha );
    BuddyAllocator_printBitmap(&allocator);



    //test 2
    printf("\n\n");
    printf("----starting small buddy test num 2----\n");
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
    printf("\n\n");
    printf("----starting small buddy test num 3----\n");
    void* ptr3 = BuddyAllocator_malloc(&allocator, 50);
    assert(ptr3 != NULL);
    BuddyAllocator_printBitmap(&allocator);

    //test 4
    printf("\n\n");
    printf("----starting small buddy test num 4----\n");
    BuddyAllocator_free(&allocator, chad);
    BuddyAllocator_printBitmap(&allocator);

    //test 5
    printf("\n\n");
    printf("----starting small buddy test num 5----\n");
    void* ptr4 = BuddyAllocator_malloc(&allocator, 150);
    assert(ptr4 != NULL);
    BuddyAllocator_printBitmap(&allocator);

    //test 6
    printf("\n\n");
    printf("----starting small buddy test num 6----\n");
    BuddyAllocator_free(&allocator, ptr1);
    BuddyAllocator_printBitmap(&allocator);
    
    //test 7
    printf("\n\n");
    printf("----starting small buddy test num 7----\n");
    BuddyAllocator_free(&allocator, ptr3);
    BuddyAllocator_printBitmap(&allocator);
    
    //test 8
    printf("\n\n");
    printf("----starting small buddy test num 8----\n");
    BuddyAllocator_free(&allocator, ptr4);
    BuddyAllocator_printBitmap(&allocator);
    
    printf("\n\n");
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
    printf("\n\n");
    printf("----starting big boy test num 1----\n");
    BuddyAllocator_printBitmap(&allocator);
    void* ptr = BuddyAllocator_malloc(&allocator, MEMORY_SIZE / 2 - sizeof(int));
    assert(ptr != NULL);
    BuddyAllocator_printBitmap(&allocator);
    
    
   // test 2
    printf("\n\n");
    printf("----starting big boy test num 2----\n");
    BuddyAllocator_free(&allocator, ptr);
    BuddyAllocator_printBitmap(&allocator);


    //test 3
    printf("\n\n");
    printf("----starting big boy test num 3----\n");
    void* ptr11 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE/4 - sizeof(int));
    assert(ptr11 != NULL);
    BuddyAllocator_printBitmap(&allocator);

    //test 4
    printf("\n\n");
    printf("----starting big boy test num 4----\n");
    void* ptr4 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE- sizeof(int)); //this shouldn't work because we're out of memory
    assert(ptr4 == NULL);
    BuddyAllocator_printBitmap(&allocator);


    //test 5
    printf("\n\n");
    printf("----starting big boy test num 5----\n");
    BuddyAllocator_free(&allocator, ptr11);
    BuddyAllocator_printBitmap(&allocator);
    
    


    //test 6
    printf("\n\n");
    printf("----starting big boy test num 6----\n");
    void* ptr6 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE- sizeof(int)); //this should work
    assert(ptr6 != NULL);
    BuddyAllocator_printBitmap(&allocator);
    

    //test 7
    printf("\n\n");
    printf("----starting big boy test num 7----\n");
    BuddyAllocator_free(&allocator, ptr6);
    BuddyAllocator_printBitmap(&allocator);



    //test 8
    printf("\n\n");
    printf("----starting big boy test num 8----\n");
    void* ptr7 = BuddyAllocator_malloc(&allocator, MEMORY_SIZE); //this shouldn't work
    assert(ptr7 == NULL);
    BuddyAllocator_printBitmap(&allocator);
    

    printf("----Large allocation tests passed.----\n");
    free(memory);
    free(bitmap_buffer);
    printf("\n\n");
    printf("-----------ending buddyAllocator_tests-----------\n");
}

void mmap_tests() {
    printf("\n\n\n");
    printf("-----------starting mmap_tests-----------\n");
    // test 1
    printf("\n\n");
    printf("----starting malloc_mmap test num 1----\n");
    int size1 = 1024;
    char* ptr1 = (char*)malloc_mmap(size1);
    assert(ptr1 != NULL);
    strcpy(ptr1, "Hello, World!");
    assert(strcmp(ptr1, "Hello, World!") == 0);
    printf("Test 1 passed\n");

    // test 2
    printf("\n\n");
    printf("----starting malloc_mmap test num 2----\n");
    void* ptr2 = malloc_mmap(0);
    assert(ptr2 == NULL);
    printf("Test 2 passed\n");

    // test 3
    printf("\n\n");
    printf("----starting malloc_mmap test num 3----\n");  
    int size3 = 1024 * 1024 * 10;  // 10 MB
    void* ptr3 = malloc_mmap(size3);
    assert(ptr3 != NULL);
    printf("Test 3 passed\n");

    // test 4
    printf("\n\n");
    printf("----starting malloc_mmap test num 4----\n");
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
    printf("\n\n");
    printf("----starting malloc_mmap test num 5----\n");
    free_mmap(ptr1);
    printf("Test 5 passed\n");
    
    //test 6
    printf("\n\n");
    printf("----starting malloc_mmap test num 6----\n");
    free_mmap(ptr2);
    printf("Test 6 passed\n");

    //test 7
    printf("\n\n");
    printf("----starting malloc_mmap test num 7----\n");
    free_mmap(ptr3);
    printf("Test 7 passed\n");
    
    //test 8
    printf("\n\n");
    printf("----starting malloc_mmap test num 8----\n");
    free_mmap(chad);
    printf("Test 8 passed\n");

    //test 9
    printf("\n\n");
    printf("----starting malloc_mmap test num 9----\n");
    void* giant = malloc_mmap((1 << 30) + 1 );
    assert(giant == NULL);
     printf("\n\n");
    printf("-----------malloc_mmap allocation tests passed.-----------\n");
}

void pseudoMalloc_tests() {
    printf("\n\n\n");
    printf("-----------starting pseudoMalloc_tests-------------\n");
    // Constants (replace these with the actual values from your implementation)
    const int BUDDY_MAX_SIZE = 1024;  // Assuming PAGE_SIZE/4, with PAGE_SIZE = 4096
    const int MAX_SIZE_MMAP = 1 << 30;  // 1GB

    // Test 1: Allocate small memory (should use buddy allocator)
    printf("\n\n");
    printf("----starting pseudoMalloc test num 1----\n");
    void* small = pseudoMalloc(100);
    assert(small != NULL);
    memset(small, 'a', 100);
    pseudoFree(small);

    // Test 2: Allocate memory just below BUDDY_MAX_SIZE
    printf("\n\n");
    printf("----starting pseudoMalloc test num 2----\n");
    void* medium = pseudoMalloc(BUDDY_MAX_SIZE - sizeof(int));
    assert(medium != NULL);
    memset(medium, 'b', BUDDY_MAX_SIZE - sizeof(int));
    pseudoFree(medium);

    // Test 3: Allocate memory just above BUDDY_MAX_SIZE (should use mmap)
    printf("\n\n");
    printf("----starting pseudoMalloc test num 3----\n");
    void* large = pseudoMalloc(BUDDY_MAX_SIZE + 1 - sizeof(int));
    assert(large != NULL);
    memset(large, 'c', BUDDY_MAX_SIZE + 1 - sizeof(int));
    pseudoFree(large);

    // Test 4: Allocate memory of size 0 (should return NULL)
    printf("\n\n");
    printf("----starting pseudoMalloc test num 4----\n");
    void* zero_size = pseudoMalloc(0);
    assert(zero_size == NULL);

    // Test 5: Allocate memory larger than MAX_SIZE_MMAP (should return NULL)
    printf("----starting pseudoMalloc test num 5----\n");
    void* too_large = pseudoMalloc(MAX_SIZE_MMAP + 1);
    assert(too_large == NULL);

    // Test 6: Allocate multiple small chunks to fill buddy allocator
    printf("\n\n");
    printf("----starting pseudoMalloc test num 6----\n");
    void* chunks[1024];
    for (int i = 0; i < 1024; i++) {
        chunks[i] = pseudoMalloc(BUDDY_MAX_SIZE-sizeof(int));
        assert(chunks[i] != NULL);
    }

    // Test 7: Attempt to allocate when buddy is full (should use mmap)
    printf("\n\n");
    printf("----starting pseudoMalloc test num 7----\n");
    void* overflow = pseudoMalloc(BUDDY_MAX_SIZE);
    assert(overflow != NULL);

    // Free all allocated chunks
    for (int i = 0; i < 1024; i++) {
        pseudoFree(chunks[i]);
    }
    pseudoFree(overflow);

    // Test 8: Multiple small allocation of the same size
    printf("\n\n");
    printf("----starting pseudoMalloc test num 8----\n");
    void* reuse1 = pseudoMalloc(50);
    void* reuse2 = pseudoMalloc(50);
    pseudoFree(reuse1);
    void* reuse3 = pseudoMalloc(50);
    assert(reuse1 == reuse3);
    pseudoFree(reuse2);
    pseudoFree(reuse3);

    // Test 9: Try to free NULL pointer 
    printf("\n\n");
    printf("----starting pseudoMalloc test num 9----\n");
    pseudoFree(NULL);

    // Test 10: Allocate memory of size 1 (minimum allocation)
    printf("\n\n");
    printf("----starting pseudoMalloc test num 10----\n");
    void* min_alloc = pseudoMalloc(1);
    assert(min_alloc != NULL);
    pseudoFree(min_alloc);
    printf("\n\n");
    printf("-----------All pseudoMalloc tests passed-----------\n");

}


int main(int argc, char *argv[]) {
    if (argc == 1) {
        // No arguments provided, default to pseudoMalloc_tests
        pseudoMalloc_tests();
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-b") == 0) {
                small_buddy_tests();
                big_buddy_tests();
            } else if (strcmp(argv[i], "-m") == 0) {
                mmap_tests();
            } else if (strcmp(argv[i], "-p") == 0) {
                pseudoMalloc_tests();
            } else {
                printf("Unknown option: %s\n", argv[i]);
            }
        }
    }
    printf("All tests passed successfully!\n");
    return 0;
}