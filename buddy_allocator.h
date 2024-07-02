#pragma once
#include "bit_map.h"

#define MAX_LEVELS 16

typedef struct {
  BitMap bitmap;
  int num_levels;
  char* memory; 
  int min_bucket_size; 
} BuddyAllocator;



void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);
