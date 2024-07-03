#pragma once
#include <stdint.h>

// simple bit array
typedef struct  {
  uint8_t *buffer;  // externally allocated buffer
  int buffer_size;
  int num_bits; 
} BitMap;

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits);

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int buffer_size, uint8_t* buffer);

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status);

// inspects the status of the bit bit_num
int BitMap_bit(BitMap* bit_map, int bit_num);

int from_level_to_bits (int level);

void BitMap_reset(BitMap* bit_map);
