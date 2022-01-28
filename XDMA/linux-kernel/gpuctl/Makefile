KERNELVER := $(shell uname -r)

topdir := $(shell cd $(src)/.. && pwd)

NVIDIA_DRIVER_PATH := $(topdir)/NVIDIA-Linux-x86_64-470.86
KBUILD_EXTRA_SYMBOLS := $(NVIDIA_DRIVER_PATH)/kernel/Module.symvers

EXTRA_CFLAGS += -I$(NVIDIA_DRIVER_PATH)/kernel/nvidia
EXTRA_CFLAGS += -fno-stack-protector
EXTRA_CFLAGS += -O2

ccflags-y += $(EXTRA_CFLAGS)

ifneq ($(KERNELRELEASE),)

obj-m := gpuctl.o
gpuctl-objs := ./src/gpuctl.o ./src/nvfs-p2p.o

else

endif

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
