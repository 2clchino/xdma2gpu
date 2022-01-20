#include "cuda.h"
#include "gpuctl.h"
#include "cdev_sgdma.h"

#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h> // posix_memalign()
#include <stdio.h>
#include <unistd.h> // read(), open(), close()
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/uio.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>


// #define IOCTL_XDMA_WRITE          _IOR('q', 8, struct xdma_data_ioctl *)
# define N 100
typedef struct xdma_data_ioctl{
  float *value;
  size_t count;
} dma_read;

typedef struct parallel_write_arg{
  int fd;
  unsigned int cmd;
  dma_read *data;
} parallel_write;

typedef struct parallel_read_arg{
  int fd;
  unsigned int cmd;
  gpudma_lock_t *data;
} parallel_read;

void* write_test(void* p){
  parallel_write* datas;
  datas = (parallel_write*)p;
  ioctl(datas->fd, datas->cmd, datas->data);
  return NULL;
}

void* read_test(void* p){
  parallel_read* datas;
  datas = (parallel_read*)p;
  ioctl(datas->fd, datas->cmd, datas->data);
  return NULL;
}
/*
void* read_test(void* p){
  parallel_write* datas;
  datas = (parallel_write*)p;
  ioctl(datas->fd, datas->cmd, datas->data);
  return NULL;
}
*/

//-----------------------------------------------------------------------------

void checkError(CUresult status);
bool wasError(CUresult status);

//-----------------------------------------------------------------------------

int main(){
  // ------------------------------------------------------------
  // Open XDMA channels

  gpudma_lock_t lock;
  gpudma_unlock_t unlock;
  gpudma_state_t *state = 0;
  int statesize = 0;
  int res = -1;
  unsigned count=0x0A000000;
  
  // char hoge[20];
  // char *addr;
  dma_read tmp;//  = { addr, sizeof(hoge)};
  pthread_t thr1, thr2;
  parallel_write write_data;
  parallel_read read_data;
  int fd_o = open("/dev/xdma0_h2c_0", O_WRONLY);
  if (fd_o < 0){
    printf("Can't open H2C.\n");
    return -1;
  }

  int fd_i = open("/dev/xdma0_c2h_0", O_RDONLY);
  if (fd_i < 0){
    printf("Can't open C2H.\n");
    return -1;
  }

  checkError(cuInit(0));
  
  int total = 0;
  checkError(cuDeviceGetCount(&total));
  fprintf(stderr, "Total devices: %d\n", total);
  
  CUdevice device;
  checkError(cuDeviceGet(&device, 0));
  
  char name[256];
  checkError(cuDeviceGetName(name, 256, device));
  fprintf(stderr, "Select device: %s\n", name);
  
  // get compute capabilities and the devicename
  int major = 0, minor = 0;
  checkError( cuDeviceComputeCapability(&major, &minor, device));
  fprintf(stderr, "Compute capability: %d.%d\n", major, minor);
  
  size_t global_mem = 0;
  checkError( cuDeviceTotalMem(&global_mem, device));
  fprintf(stderr, "Global memory: %llu MB\n", (unsigned long long)(global_mem >> 20));
  if(global_mem > (unsigned long long)4*1024*1024*1024L)
    fprintf(stderr, "64-bit Memory Address support\n");
  
  CUcontext  context;
  checkError(cuCtxCreate(&context, 0, device));
  
  float *arr1, *arr2;
  size_t n_byte = N * sizeof(float);
  arr1 = (float *)malloc(n_byte);
  arr2 = (float *)malloc(n_byte);
  for (int i = 0; i < N; i++){
    arr1[i] = (float)(i + 1);
  }
  size_t size = sizeof(arr1);
  CUdeviceptr dptr = 0;
  unsigned int flag = 1;
  unsigned char *h_odata = NULL;
  h_odata = (unsigned char *)malloc(size);
  
  CUresult status = cuMemAlloc(&dptr, size);
  if(wasError(status)) {
    goto do_free_context;
  }
  
  fprintf(stderr, "Allocate memory address: 0x%llx\n",  (unsigned long long)dptr);
  
  status = cuPointerSetAttribute(&flag, CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, dptr);
  if(wasError(status)) {
    goto do_free_memory;
  }
  
  fprintf(stderr, "Press enter to lock\n");

  lock.addr = dptr;
  lock.size = size;
  // ------------------------------------------------------------
  // Send and receive data
  
  // strcpy(hoge, "Hello");
  // addr = &(hoge[0]);
  // ioctl(fd_o, IOCTL_XDMA_GPU_WRITE, &lock);
  // ioctl(fd_i, IOCTL_XDMA_GPU_READ, &lock);
  // printf("Hello\n");
  printf("addr: %ld, size: %ld\n", lock.addr, lock.size);
  tmp = { arr1, sizeof(arr1)};
  write_data = { fd_o, IOCTL_XDMA_WRITE, &tmp};
  read_data = { fd_i, IOCTL_XDMA_GPU_READ, &lock};
  pthread_create( &thr1, NULL, write_test, (void*)(&write_data));
  pthread_create( &thr2, NULL, read_test, (void*)(&read_data));
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);
  pthread_detach(thr1);
  pthread_detach(thr2);

  sleep(5);
  printf("addr: %ld, size: %ld\n", lock.addr, lock.size);
  cuMemcpyDtoH(arr2, lock.addr, lock.size);
  for (int i = 0; i < N; i++){
    printf("%f -> %f\n",arr1[i], arr2[i]);
  }
  
  // ------------------------------------------------------------
  // Clean up
  
  /*
 do_unlock:
  unlock.handle = lock.handle;
  res = ioctl(fd, IOCTL_GPUMEM_UNLOCK, &unlock);
  if(res < 0) {
    fprintf(stderr, "Error in IOCTL_GPUDMA_MEM_UNLOCK\n");
    goto do_free_state;
  }
  */
 do_free_state:
  free(state);
 do_free_attr:
  flag = 0;
  cuPointerSetAttribute(&flag, CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, dptr);
  
 do_free_memory:
  cuMemFree(dptr);
  
 do_free_context:
  cuCtxDestroy(context);
  
  close(fd_o);
  close(fd_i);
  
  return 0;
}

// -------------------------------------------------------------------

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
