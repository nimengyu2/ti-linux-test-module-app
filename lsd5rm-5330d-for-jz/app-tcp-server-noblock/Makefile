export CC=arm-none-linux-gnueabi-gcc
tcp-server-noblock:
TARGET=tcp-server-noblock
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)	
clean:
	rm -rf *.o

