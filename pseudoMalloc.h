#pragma once
#include "buddy_allocator.h"

typedef struct {
    int PAGE_SIZE;   
    int BUDDY_MAX_SIZE;
    int KEY_VALUE;
    int MAX_SIZE_MMAP;
    int BUDDY_MEMORY_SIZE;
    int MIN_BUCKET_SIZE;
    int NUM_LEVELS;
    int BITMAP_SIZE_BYTES;
    BuddyAllocator buddy;
} PseudoMallocStruct;

void pseudoMallocInit(void) __attribute__((constructor));
void* PseudoMalloc(int size);
void PseudoFree(void* memory);