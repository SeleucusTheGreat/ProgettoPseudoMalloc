#include "bit_map.h"
#include "buddy_allocator.h"

int levelIdx(size_t idx){
  return (int)floor(log2(idx));
};

int buddyIdx(int idx){
  if (idx&0x1){
    return idx-1;
  }
  return idx+1;
}

int parentIdx(int idx){
  return idx/2;
}

int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}


void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         char* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size) {
                         
                         

}

