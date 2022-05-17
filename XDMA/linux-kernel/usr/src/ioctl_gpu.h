#ifndef _IOCTL_GPU_
#define _IOCTL_GPU_
#include "cuda.h"
#include "gpuctl.h"
#include "cdev_sgdma.h"

#include <unistd.h> // read(), write(), open(), close()
#include <dirent.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/uio.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

int xdma2gpu(int fd, uint64_t addr, uint64_t size);
int gpu2xdma(int fd, uint64_t addr, uint64_t size);
void checkError(CUresult status);
bool wasError(CUresult status);
#endif
