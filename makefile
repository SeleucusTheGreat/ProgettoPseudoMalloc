CC = gcc

CFLAGS = -Wall -Wextra -Werror -g
SRC = bit_map.c buddy_allocator.c tests.c malloc_mmap.c pseudoMalloc.c
HEADERS = bit_map.h buddy_allocator.h malloc_mmap.h pseudoMalloc.h


OBJ = $(SRC:.c=.o)

TARGET = tests


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) 

.PHONY: clean
