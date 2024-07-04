#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc_mmap.h"

#define MAX_SIZE 1 << 30 //1 Gbyte more or less

void* malloc_mmap(int size) {
    if (size == 0) return NULL;
    if (size > MAX_SIZE) {
        return NULL;
        printf("memory required is too large ");
    }

    void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
    if (memory == MAP_FAILED) {
        return NULL;
        printf("there was an error during the allocation of the mmap for the malloc ");
    }
    return memory;
    
}

void free_mmap(void* memory, int size) {
    if ((size == 0) | (memory == NULL))
        return;
    
    if (munmap(memory, size) != 0) {
        perror("free failed");
        return;
    }
}