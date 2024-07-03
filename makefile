CC = gcc

CFLAGS = -Wall -Wextra -Werror -g
SRC = bit_map.c buddy_allocator.c buddy_allocator_tests.c
HEADERS = bit_map.h buddy_allocator.h


OBJ = $(SRC:.c=.o)

TARGET = buddy_allocator_tests


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
