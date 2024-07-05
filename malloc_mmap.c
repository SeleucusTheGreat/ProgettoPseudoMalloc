#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc_mmap.h"
#include <errno.h>
#include <string.h>

#define MAX_SIZE 1 << 30 //1 Gbyte more or less
#define KEY_VALUE 2097151

void* malloc_mmap(int size) {
    if (size == 0) return NULL;
    if (size > MAX_SIZE) {
        return NULL;
        printf("memory required is too large ");
    }

    void* memory = mmap(NULL, size + sizeof(int), PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
    *(int*) (memory) = size + KEY_VALUE;
    if (memory == MAP_FAILED) {
        return NULL;
        printf("there was an error during the allocation of the mmap for the malloc ");
    }
    return memory + sizeof(int);
    
}

void free_mmap(void* memory) {
    if (memory==NULL)
        return;


    char* block_start =(char*) memory;
    block_start = block_start - sizeof(int);
    int size = *(int*)block_start;
    if(size <= KEY_VALUE){
        printf("this block should belong to the buddy allocator");
        return;
    }
    size = size - KEY_VALUE;
    
    if ((size == 0)){
        printf("this memory shouldn't be size zero");
        return;
    }
    if (munmap(block_start, size) != 0) {
        fprintf(stderr, "free failed because %d: %s\n", size, strerror(errno));
        return;
    }

    printf("memory of size %d freed successfully\n", size);
}