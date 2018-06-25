# Based on the Analog linux kernel sources
# Obtain the Analog sources from:
# https://github.com/analogdevicesinc/linux/tree/2015_R1

# Define path to your Linaro kernel
KERNELDIR=/root/linux-2015_R1/

obj-m += kernel_mode.o

all: 
	make -C $(CFLAGS) $(KERNELDIR) M=$(PWD) modules

install:
	insmod kernel_mode.ko
	chmod -fR 777 /dev/sw_firewall


clean:
	make -C $(KERNELDIR) M=$(PWD) clean 
	rmmod kernel_mode.ko
	dmesg -C
	tee /var/log/kern.log < /dev/null
