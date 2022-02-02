#include "ioctl_gpu.h"
int xdma2gpu(int fd, uint64_t addr, uint64_t size){
  gpudma_lock_t lock;
  lock.addr = addr;
  lock.size = size;
  if (fd != 4)
    return -1;
  ioctl(fd, IOCTL_XDMA_GPU_READ, &lock);
  return 0;
}

int gpu2xdma(int fd, uint64_t addr, uint64_t size){
  gpudma_lock_t lock;
  lock.addr = addr;
  lock.size = size;
  if (fd != 3)
    return -1;
  ioctl(fd, IOCTL_XDMA_GPU_WRITE, &lock);
  return 0;
}

//-----------------------------------------------------------------------------
