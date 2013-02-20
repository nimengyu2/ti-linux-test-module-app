export CC=arm-none-linux-gnueabi-gcc
open-write-sync:
TARGET=open-write-sync
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)	
	rm -rf *.o
clean:
	rm -rf *.o

