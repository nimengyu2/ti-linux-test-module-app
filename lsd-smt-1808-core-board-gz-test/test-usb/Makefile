export CC=arm-none-linux-gnueabi-gcc
test_usb_sata:
TARGET=test_usb_sata
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

