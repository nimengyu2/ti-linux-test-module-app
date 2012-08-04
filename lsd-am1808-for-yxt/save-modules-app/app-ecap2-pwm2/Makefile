export CC=arm-none-linux-gnueabi-gcc
ecap2-pwm2:
TARGET=ecap2-pwm2
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)	
	rm -rf *.o
clean:
	rm -rf *.o

