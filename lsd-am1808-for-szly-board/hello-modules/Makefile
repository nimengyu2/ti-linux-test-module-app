
obj-m := hello.o  
PWD := $(shell pwd)  
KERNELDIR := /am1808/linux-03.20.00.12
CROSS_ARCH := ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-
modules:  
	$(MAKE) $(CROSS_ARCH) -C $(KERNELDIR) M=$(PWD) modules	
	rm -f *.o *.mod.c modules.order Module.symvers
clean:
	rm -f *.o *.mod.c modules.order Module.symvers
