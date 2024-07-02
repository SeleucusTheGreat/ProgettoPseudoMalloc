CC= gcc
CFLAGS= --std=gnu99 -Wall 
AR= ar


OBJS= pool_allocator.o


HEADERS= pool_allocator.h


LIBRARY= pool_library.a


BIN= test_pool_allocator


.phony: all clean


all: $(LIBRARY) $(BIN)


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


$(LIBRARY): $(OBJS)
	$(AR) -rcs $@ $^


$(BIN): test_pool_allocator.o $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm *.o *~ $(LIBRARY) $(BIN) 

