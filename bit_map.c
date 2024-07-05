#include <assert.h>
#include "bit_map.h"
#include <stdio.h>

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 + (bits%8)!=0;
}



// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int buffer_size , uint8_t* buffer){
  bit_map->buffer=buffer;
  bit_map->num_bits=buffer_size*8;
  bit_map->buffer_size=buffer_size;
  for (int i = 0; i < bit_map->buffer_size; ++i) {
        bit_map->buffer[i] = 0;
    }
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>> 3; 
  //printf("the byte is %d\n",byte_num );
  int bit_in_byte=bit_num&0x07;

  //printf("the bit in byte should be %d\n",bit_in_byte );
  if (status) {
    bit_map->buffer[byte_num] |= (1<<(bit_in_byte));
  } else {
    bit_map->buffer[byte_num] &= ~(1<<(bit_in_byte));
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(BitMap* bit_map, int bit_num){
  int byte_num=bit_num>> 3; 
  int bit_in_byte=bit_num&0x07; 
  
  int result = (bit_map->buffer[byte_num] & (1<<(bit_in_byte)))!=0;

  return result; 
}

//put all the bits of the bitmap to
void BitMap_reset(BitMap* bit_map) {
    for (int i = 0; i < bit_map->buffer_size; ++i) {
        bit_map->buffer[i] = 0;
    }
}
