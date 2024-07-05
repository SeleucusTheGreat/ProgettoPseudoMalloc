#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include "malloc_mmap.h"
#include "buddy_allocator.h"
#include "pseudoMalloc.h"

static PseudoMallocStruct str;


void pseudoMallocInit() __attribute__((constructor));

void pseudoMallocInit() {
    str.PAGE_SIZE = sysconf(_SC_PAGESIZE);
    str.BUDDY_MAX_SIZE = str.PAGE_SIZE/4;
    str.KEY_VALUE = 2097151; //index number
    str.MAX_SIZE_MMAP = 1 << 30; // 1Gb
    str.BUDDY_MEMORY_SIZE = (1 << 20); // 1Mb 
    str.NUM_LEVELS = 21;
    str.BITMAP_SIZE_BYTES = 262144;
    str.MIN_BUCKET_SIZE = 1;
    char* memory = (char*)malloc(str.BUDDY_MEMORY_SIZE);
    uint8_t* bitmap_buffer = (uint8_t*)malloc(str.BITMAP_SIZE_BYTES);
    BuddyAllocator_init(&str.buddy, str.NUM_LEVELS, bitmap_buffer, str.BITMAP_SIZE_BYTES , memory, str.BUDDY_MEMORY_SIZE , str.MIN_BUCKET_SIZE);

    printf("page size is %d\n",str.PAGE_SIZE); 
}





