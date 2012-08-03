# CROSS=arm-none-linux-gnueabi-

all:mkcgifs

mkcgifs:mkcgifs.c
	$(CROSS)gcc -o mkcgifs mkcgifs.c
	$(CROSS)strip mkcgifs
clean:
	@rm -vf mkcgifs *.o *~
