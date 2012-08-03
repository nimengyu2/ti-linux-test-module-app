obj-m := busior.o  
PWD := $(shell pwd)  
KERNELDIR := /am1808/linux-03.20.00.12
CROSS_ARCH := ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-
#EXTRA_CFLAGS += -I/am1808/linux-03.20.00.12/arch/arm/mach-davinci/include
#CFLAGS	+= -I$(KERNELDIR)/arch/arm/mach-davinci/include
EXTRA_CFLAGS	+= -I$(KERNELDIR)/drivers/serial/omapl_pru/pru/hal/common/include
modules:  
	$(MAKE) $(CROSS_ARCH) $(EXTRA_CFLAGS)  -C $(KERNELDIR)  M=$(PWD) modules
	#$(MAKE) $(CROSS_ARCH) -C $(KERNELDIR)  M=$(PWD) modules	
	rm -f *.o *.mod.c modules.order Module.symvers
clean:
	rm -f *.o *.mod.c modules.order Module.symvers
