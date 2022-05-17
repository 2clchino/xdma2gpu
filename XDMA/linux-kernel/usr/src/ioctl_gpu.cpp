#include "ioctl_gpu.h"

//-----------------------------------------------------------------------------

int xdma2gpu(int fd, uint64_t addr, uint64_t size){
  gpudma_lock_t lock;
  lock.addr = addr;
  lock.size = size;
  if (fd != 4)
    return -1;
  return ioctl(fd, IOCTL_XDMA_GPU_READ, &lock);
}

int gpu2xdma(int fd, uint64_t addr, uint64_t size){
  gpudma_lock_t lock;
  lock.addr = addr;
  lock.size = size;
  if (fd != 3)
    return -1;
  return ioctl(fd, IOCTL_XDMA_GPU_WRITE, &lock);
}

//-----------------------------------------------------------------------------

void checkError(CUresult status)
{
    if(status != CUDA_SUCCESS) {
        const char *perrstr = 0;
        CUresult ok = cuGetErrorString(status,&perrstr);
        if(ok == CUDA_SUCCESS) {
            if(perrstr) {
                fprintf(stderr, "info: %s\n", perrstr);
            } else {
                fprintf(stderr, "info: unknown error\n");
            }
        }
        exit(0);
    }
}

//-----------------------------------------------------------------------------

bool wasError(CUresult status)
{
    if(status != CUDA_SUCCESS) {
        const char *perrstr = 0;
        CUresult ok = cuGetErrorString(status,&perrstr);
        if(ok == CUDA_SUCCESS) {
            if(perrstr) {
                fprintf(stderr, "info: %s\n", perrstr);
            } else {
                fprintf(stderr, "info: unknown error\n");
            }
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
