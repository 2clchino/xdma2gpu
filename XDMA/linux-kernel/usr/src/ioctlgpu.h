#include "gpuctl.h"
#include "cdev_sgdma.h"

int xdma2gpu(int fd, uint64_t addr, uint64_t size){
  gpudma_lock_t lock;
  lock.addr = addr;
  lock.size = size;
  ioctl(fd, IOCTL_XDMA_GPU_READ, &lock);
}

int gpu2xdma(int fd, uint64_t addr, uint64_t size){
  gpudma_lock_t lock;
  lock.addr = addr;
  lock.size = size;
  ioctl(fd, IOCTL_XDMA_GPU_WRITE, &lock);
}
