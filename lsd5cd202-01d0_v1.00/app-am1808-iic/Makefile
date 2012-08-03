CROSS=arm-none-linux-gnueabi-

all: i2c_rx8025t_test

i2c_rx8025t_test_wr:i2c_rx8025t_test_wr.c
	$(CROSS)gcc -o i2c_rx8025t_test_wr i2c_rx8025t_test_wr.c
i2c_rx8025t_test_rd:i2c_rx8025t_test_rd.c
	$(CROSS)gcc -o i2c_rx8025t_test_rd i2c_rx8025t_test_rd.c

#$(CROSS)strip i2c_rx8025t_test
clean:
	@rm -vf i2c_rx8025t_test *.o *~
