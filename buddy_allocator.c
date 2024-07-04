#include "bit_map.h"
#include "buddy_allocator.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int levelIdx(int idx){
  return (int)floor(log2(idx));
}

int buddyIdx(int idx){
  if (idx ==1)
    return 2;
  if (idx ==2)
    return 1;

  if (idx & 0x1){
      return idx - 1;
  }
  
  return idx + 1;
}


int parentIdx(int idx){
  if (idx == 1)
    return 0;
  
  if (idx == 2)
    return 0;

  return idx / 2;
}



int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}


int from_level_to_bits (int level){
  return 1 << (level + 1);
}





void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         uint8_t* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size) {

	assert(num_levels <= MAX_LEVELS );
	
	
	alloc -> num_levels = num_levels;
	alloc -> memory = memory;
	alloc -> min_bucket_size = min_bucket_size;
	
       
  BitMap* bitmap = &alloc->bitmap;
  BitMap_init(bitmap, buffer_size,  (uint8_t*) bitmap_buffer ); //we initialize the bitmap
        
        
  //this should start the root at 1 and everything should be 0
  BitMap_reset(bitmap);
  BitMap_setBit(bitmap, 0, 1);

  


	printf("BUDDY INITIALIZING\n");
  printf("\tlevels: %d\n", num_levels);
  printf("\tbitmap size: %d bits or %d bytes\n", bitmap->num_bits,bitmap->buffer_size);
  printf("\tbucket size:%d\n", min_bucket_size);
  printf("\tmanaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);

  

}

int find_free_buddy(BuddyAllocator* alloc, int level) { //return the first free buddy starting from left of a certain level
    //printf("looking for buddies at level %d\n", level);
    int start_idx = (1 << level) -1 ;
  	int end_idx = (1 << (level + 1)) - 2;

 	for (int i = start_idx; i <= end_idx; i++) {
    		if (BitMap_bit(&alloc->bitmap, i)) {
      		return i;
    		}
 	}
	return -1;
}

void split_buddy(BuddyAllocator* alloc, int idx) { //split the buddy and put 0 in the current buddy
  if (idx==0) {
    BitMap_setBit(&alloc->bitmap, idx, 0);
    BitMap_setBit(&alloc->bitmap, 1, 1);
    BitMap_setBit(&alloc->bitmap, 2, 1);
  }
  
  if (idx!=0){
  BitMap_setBit(&alloc->bitmap, idx, 0);
  BitMap_setBit(&alloc->bitmap, idx * 2, 1);
  BitMap_setBit(&alloc->bitmap, idx * 2 + 1, 1);
  }
}



int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
    if (level < 0)
        return 0;
    assert(level <= alloc->num_levels);
    int index = find_free_buddy(alloc, level);
    if (index == -1) { // No buddies on this level
        if (level == 0) // We are at the root
            return 0; // No space available
        //printf("I haven't found a buddy at level %d\n", level);
        // Get a buddy from a higher level
        index = BuddyAllocator_getBuddy(alloc, level - 1);
        
        
        if (index == -1) // No buddy available at any higher level
            return 0;

        // Split the found buddy
        split_buddy(alloc, index);
        
        // We get the index of the left child
        if(index==0) {
          index = 1;
        }
        if (index!=0) {
        index = index * 2;
        }
    }
    //printf("I've found a buddy at level %d with index %d\n", level, index);
    // Give the index of the buddy
    return index;
}




void *BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {

  
	int memory_size = (int) (1<<alloc->num_levels)*alloc->min_bucket_size; //get the size of the memory total
	assert(size <= memory_size -  (int) sizeof(int) );//check if we actually have the right amount of memory
	
	 int level = (int)floor(log2((double)memory_size / (size + sizeof(int)))); //we get the correct level 
	
	// if the level is too small, we pad it to max
  	if (level>alloc->num_levels){
    		level=alloc->num_levels;
    	}
    	
    	printf("requested: %d bytes, level %d \n", size, level);
    	
    	int index = BuddyAllocator_getBuddy(alloc, level); //get the index of the best buddy we can get
    	
    	if (index == -1) {
    		return 0; // no buddy no party 
	    }
    	printf("I'm going to set a bit in index:%d considering my bitmap has %d bits\n", index, from_level_to_bits(alloc->num_levels));
    	BitMap_setBit(&alloc->bitmap, index, 0); //set the bit of the index 
    	
    	// we get the block size of the current level
    	int block_size = alloc->min_bucket_size * (1 << (alloc->num_levels - level)); 
    	
    	
    	int start_index = startIdx(index); //we get the start index
    	
    	
    	
    	char* block_start = alloc->memory + (start_index * block_size); //the block we want to release
    	
    	*(int*)(block_start) = index; //we put the index inside the first sizeof(int) bytes
      
      return block_start + sizeof(int); // but we return the block after the part we saved the index
    	
}

void merge_buddies(BuddyAllocator* alloc, int index) {
  if (index < 0) return; //no buddy to merge :(

  
  int buddy = buddyIdx(index); //his buddy
  int parent = parentIdx(index); //buddy's daddy
  
  //assuming the we have the block and his buddy is available
  if (BitMap_bit(&alloc->bitmap, buddy)) { 
    BitMap_setBit(&alloc->bitmap, index, 0); //buddy will be available again
    BitMap_setBit(&alloc->bitmap, buddy, 0);//buddy's buddy will be available again
    BitMap_setBit(&alloc->bitmap, parent, 1); //daddy is available
    merge_buddies(alloc, parent); //recursive call for the daddy
  }
}

void BuddyAllocator_free(BuddyAllocator* alloc, void* mem) {
  if (!mem) return; //fake memory block given
  
  
  char* block_start =(char*) mem; //the block in question
  block_start = block_start - sizeof(int);  
  int index = *(int*)block_start; //but we want to know the index
  int level = levelIdx(index);
  printf("this block is no more at index: %d , at this level %d\n", index, level);
  
  BitMap_setBit(&alloc->bitmap, index, 1); //set the buddy to available 
  merge_buddies(alloc, index); //start merging 
  
  
}



void BuddyAllocator_printBitmap(BuddyAllocator* alloc) {
    printf("Buddy Allocator Bitmap Situation:\n");
    for (int level = 0; level < alloc->num_levels; ++level) {
        int start_idx = (1 << level) -1 ;
        int end_idx = (1 << (level + 1)) - 2;
        printf("level %d    ", level);
        
        for (int i = start_idx; i <= end_idx; ++i) {
            if (BitMap_bit(&alloc->bitmap, i)) {
              printf("1"); 
              
            }
        }

        

        printf("\n");
    }
}

