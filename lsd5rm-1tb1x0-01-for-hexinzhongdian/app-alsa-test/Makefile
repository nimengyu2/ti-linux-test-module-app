export CC=arm-none-linux-gnueabi-g++
alsa-test:
TARGET=alsa-test
%.o: %.c
	$(CC) $(CFLAGS) -c $< -lasound -o $@  -I/tar-tmp/alsa/alsa-ouput/include -L/tar-tmp/alsa/alsa-ouput/lib
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -lasound -o $(TARGET) -I/tar-tmp/alsa/alsa-ouput/include -L/tar-tmp/alsa/alsa-ouput/lib
	rm -rf *.o
clean:
	rm -rf *.o

