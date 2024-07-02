#include "bit_map.h"
#include "buddy_allocator.h"
#include <assert.h>
#include <math.h>

int levelIdx(size_t idx){
  return (int)floor(log2(idx));
}

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


int from_level_to_bits (int level){
  return 1 << (levels + 1);
}


void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         char* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size) {

	assert(num_levels < MAX_LEVELS );
	
	
	alloc -> num_levels = num_levels;
	alloc -> memory = memory;
	alloc -> min_bucket_size = min_bucket_size;
	
        int bitmap_size = from_level_to_bits(num_level);
        
        assert (buffer_size >= bitmap_size);
        
        BitMap* bit_map = alloc->bit_map
        BitMap_init(bit_map, bitmap_size,  (uint8_t*) bitmap_buffer ); //we initialize the bitmap
        
        //this should start the root at 1 and everything should be 0
        BitMap_setBit(bit_map, 1, 1);

	printf("BUDDY INITIALIZING\n");
  	printf("\tlevels: %d", num_levels);
  	printf("\tbitmap size: %d bits\n", bitmap_size);
  	printf("\tbucket size:%d\n", min_bucket_size);
  	printf("\tmanaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);

}

int find_free_buddy(BuddyAllocator* alloc, int level) { //return the first free buddy starting from left of a certain level
  	int start = 1 << level;
  	int end = 1 << (level + 1);
  
 	for (int i = start; i < end; i++) {
    		if (BitMap_bit(&alloc->bitmap, i)) {
      		return i;
    		}
 	}
	return 0;
}

void split_buddy(BuddyAllocator* alloc, int idx) { //split the buddy and put 0 in the current buddy
  BitMap_setBit(&alloc->bitmap, idx, 0);
  BitMap_setBit(&alloc->bitmap, idx * 2, 1);
  BitMap_setBit(&alloc->bitmap, idx * 2 + 1, 1);
}



int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
    if (level < 0)
        return 0;
    assert(level <= alloc->num_levels);

    int index = find_free_buddy(alloc, level);
    if (index == 0) { // No buddies on this level
        if (level == 0) // We are at the root
            return 0; // No space available
        
        // Get a buddy from a higher level
        index = BuddyAllocator_getBuddy(alloc, level - 1);
        
        if (index == 0) // No buddy available at any higher level
            return 0;

        // Split the found buddy
        split_buddy(alloc, index);
        
        // We get the index of the left child
        index = index * 2;
    }

    // Give the index of the buddy
    return index;
}




void *BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {

	int memory_size = (1<<alloc->num_levels)*alloc->min_bucket_size; //get the size of the memory total
	assert(size <= memory_size -  sizeof(int) );//check if we actually have the right amount of memory
	
	 int level = (int)floor(log2((double)memory_size / (size + sizeof(int)))); //we get the correct level 
	
	// if the level is too small, we pad it to max
  	if (level>alloc->num_levels){
    		level=alloc->num_levels;
    	}
    	
    	printf("requested: %d bytes, level %d \n", size, level);
    	
    	int index = BuddyAllocator_getBuddy(alloc, level); //get the index of the best buddy we can get
    	
    	if (index == 0) {
    		return NULL; // no buddy no party 
	}
    	
    	BitMap_setBit(&alloc->bitmap, index, 0); //set the bit of the index 
    	
    	// we get the block size of the current level
    	int block_size = alloc->min_bucket_size * (1 << (alloc->num_levels - level)); 
    	
    	
    	int start_index = startIdx(index); //we get the start index
    	
    	printf ("imma bout to release the %d index with the %d block size",index,block_size);
    	
    	char* block_start = alloc->memory + (start_index * block_size); //the block we want to release
    	
    	*(int*)(block_start) = index; //we put the index inside the first sizeof(int) bytes
    	
    	return block_start + sizeof(int); // but we return the block after the part we saved the index
    	
}

void merge_buddies(BuddyAllocator* alloc, int idx) {
  if (idx <= 1) return; //no buddy to merge :(
  
  int buddy = buddyIdx(idx); //his buddy
  int parent = parentIdx(idx); //buddy's daddy
  
  //assuming the we have the block and his buddy is available
  if (BitMap_bit(&alloc->bitmap, buddy)) { 
    BitMap_setBit(&alloc->bitmap, idx, 0); //buddy will be available again
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
  
  printf("this block is no more: %d\n", index);
  
  BitMap_setBit(&alloc->bitmap, index, 1); //set the buddy to available 
  merge_buddies(alloc, idx); //start merging 
  
  
}



