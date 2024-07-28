obj-m += KernelModule.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC := $(shell which gcc)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
