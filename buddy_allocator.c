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


int from_level_to_bits (int level){
  return 1 << (num_levels + 1);
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
                         
        BitMap_init(bit_map, bitmap_size,  (uint8_t*) bitmap_buffer );
        
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



int BuddyAllocator_getIndex (BuddyAllocator* alloc, int level ) {
	if (level<0)
    	return 0;
  	assert(level <= alloc->num_levels);
  	
  	
  	if (int index = find_free_buddy(alloc,level)==0 )  {// no buddies on this level
  		if (level = 0) // we are at the root
      			return 0; // no space avariable
      		BuddyAllocator_getBuddy(alloc, level-1);
      		
      		split_buddy(BuddyAllocator* alloc, index); //we've found a free buddy and we split it 
      			
  	}
  	
}







