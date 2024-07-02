#pragma once
#include "bit_map.h"

#define MAX_LEVELS 

typedef struct {
  BitMap bitmap;
  int num_levels;
  char* memory; 
  int min_bucket_size; 
} BuddyAllocator;

void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         char* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size);

void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);




