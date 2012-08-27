CROSS=arm-none-linux-gnueabi-

all: rtc-wr rtc-rd

rtc-wr:rtc-wr.c
	$(CROSS)gcc -o rtc-wr rtc-wr.c
rtc-rd:rtc-rd.c
	$(CROSS)gcc -o rtc-rd rtc-rd.c

#$(CROSS)strip i2c_rx8025t_test
clean:
	@rm -vf i2c_rx8025t_test *.o *~ rtc-rd rtc-wr
