CROSS=arm-none-linux-gnueabi-

all: msp430-iic

msp430-iic:msp430-iic.c
	$(CROSS)gcc -o msp430-iic msp430-iic.c

#$(CROSS)strip i2c_rx8025t_test
clean:
	@rm -vf msp430-iic *.o *~
