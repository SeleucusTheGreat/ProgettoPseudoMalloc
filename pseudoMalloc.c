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

void* pseudoMalloc(int size) {
    void* memory;
    if (size<=0) return NULL;
    if (size>str.MAX_SIZE_MMAP)  {printf("too much memory asked\n"); return NULL; }

    if (size >str.BUDDY_MAX_SIZE) {  //use the mmap
        return malloc_mmap(size);

    }
    else {  //use the buddy allocator 

        memory =  BuddyAllocator_malloc(&str.buddy, size);
        
        
        if (memory==NULL) {   // if the buddy is full then use the mmap 
            printf("the buddy is out of memory\n");
            return malloc_mmap(size);
        } else {
            return memory;
        }
    }
}

void pseudoFree(void* memory){
    if (memory==NULL) return;
    char* block_start =(char*) memory; 
    block_start = block_start - sizeof(int);  
    int value = *(int*)block_start; 

    if(value <= str.KEY_VALUE ) {
        BuddyAllocator_free(&str.buddy, memory);
    }
    else {
        free_mmap(memory);
    }

}


