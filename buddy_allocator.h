#pragma once
#include "bit_map.h"

#define MAX_LEVELS 32

typedef struct {
  BitMap bitmap;
  int num_levels;
  char* memory; 
  int memory_size;
  int min_bucket_size; 

} BuddyAllocator;

void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         uint8_t* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int memory_size,
                         int min_bucket_size);

void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);


void BuddyAllocator_printBitmap(BuddyAllocator* alloc);

