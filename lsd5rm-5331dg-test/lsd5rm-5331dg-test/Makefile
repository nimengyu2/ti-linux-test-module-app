export CC=arm-none-linux-gnueabi-gcc
lsd5rm-5331dg-test:
TARGET=lsd5rm-5331dg-test
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	rm -rf $(TARGET)
	$(CC) $(OBJS) -o $(TARGET)	
	rm -rf *.o 
clean:
	rm -rf *.o
	rm -rf $(TARGET)

