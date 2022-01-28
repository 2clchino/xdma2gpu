PHONY = clean all
TARGET_NAME = gpu_direct

all: $(TARGET_NAME)

ROOT_DIR = $(shell pwd)

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc

#NVIDIA_DRIVER_PATH := $(HOME)/gpudma/nvidia
NVIDIA_DRIVER_PATH := ../NVIDIA-Linux-x86_64-470.86
CUDADIR:=/opt/nvidia/hpc_sdk/Linux_x86_64/21.9/cuda/11.4/targets/x86_64-linux


INCDIR := . $(CUDADIR)/include ../gpuctl/src ../xdma $(NVIDIA_DRIVER_PATH)/kernel/nvidia
INCLUDE := $(addprefix -I, $(INCDIR))
CFLAGS := -D__LINUX__ -g -Wall $(INCLUDE)
SRCFILE := $(wildcard *.cpp) 
OBJFILE := $(patsubst %.cpp,%.o, $(SRCFILE))

#LDFLAGS := -Wl,-rpath,$(CUDADIR)/lib64/stubs -L"$(CUDADIR)/lib64/stubs" -lcuda
#LDFLAGS :=  -L/usr/local/cuda/lib64 -lcudart -L/usr/local/cuda/lib64/stubs -lcuda $(LIBS)
LDFLAGS +=  -L$(CUDADIR)/lib64 -lcuda -lpthread
#LDFLAGS +=  -L$(CUDADIR)/lib64 -lcudart
LDFLAGS +=  -lstdc++

$(TARGET_NAME): $(OBJFILE)
	$(LD) $(notdir $^) -o $(TARGET_NAME) $(LDFLAGS)
	cp $(TARGET_NAME) ../bin

%.o: %.cpp
	$(CC) $(CFLAGS) -c -MD $<

include $(wildcard *.d)

ftog:	$(patsubst %.cpp,%.o, ./src/ioctl_parallel_test_ftog.cpp)
	$(LD) $(notdir $^) -o ftog $(LDFLAGS)
	cp ftog ../bin

gtof:	$(patsubst %.cpp,%.o, ./src/ioctl_parallel_test_gtof.cpp)
	$(LD) $(notdir $^) -o gtof $(LDFLAGS)
	cp gtof ../bin

clean:
	rm -f *.o *~ core
	rm -f *.d *~ core
	rm -f $(TARGET_NAME)
	rm -f gtof
	rm -f ftog

distclean:
	rm -f *.o *~ core
	rm -f *.d *~ core
	rm -f $(TARGET_NAME)

src:
	@echo $(SRCFILE)
	@echo $(OBJFILE)
