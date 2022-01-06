KERNELVER := $(shell uname -r)

topdir := $(shell cd $(src)/.. && pwd)

NVIDIA_DRIVER_PATH := $(topdir)/NVIDIA-Linux-x86_64-470.74
KBUILD_EXTRA_SYMBOLS := $(NVIDIA_DRIVER_PATH)/kernel/Module.symvers

EXTRA_CFLAGS += -I$(NVIDIA_DRIVER_PATH)/kernel/nvidia

ccflags-y += $(EXTRA_CFLAGS)

obj-m := gpuctl.o
gpuctl-objs := ./src/gpuctl.o ./src/nvfs-p2p.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
