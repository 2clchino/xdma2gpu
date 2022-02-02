#include "cuda.h"
#include "ioctl_gpu.h"

#include <unistd.h> // read(), open(), close()
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/uio.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

# define N 32 * 1024 * 1024

//-----------------------------------------------------------------------------

void checkError(CUresult status);
bool wasError(CUresult status);

//-----------------------------------------------------------------------------

int main(){
  // ------------------------------------------------------------
  // Open XDMA channels

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

  size_t global_mem = 0;
  checkError( cuDeviceTotalMem(&global_mem, device));
  fprintf(stderr, "Global memory: %llu MB\n", (unsigned long long)(global_mem >> 20));
  if(global_mem > (unsigned long long)4*1024*1024*1024L)
    fprintf(stderr, "64-bit Memory Address support\n");
  
  CUcontext  context;
  checkError(cuCtxCreate(&context, 0, device));
  
  int *arr1, *arr2;
  size_t n_byte = N * sizeof(int);
  arr1 = (int *)malloc(n_byte);
  arr2 = (int *)malloc(n_byte);
  for (int i = 0; i < N; i++){
    arr1[i] = (int)(i + 1);
  }
  CUdeviceptr dptr = 0;
  unsigned int flag = 1;
  
  CUresult status = cuMemAlloc(&dptr, n_byte);
  if(wasError(status)) {
    goto do_free_context;
  }
  
  fprintf(stderr, "Allocate memory address: 0x%llx\n",  (unsigned long long)dptr);
  status = cuPointerSetAttribute(&flag, CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, dptr);
  if(wasError(status)) {
    goto do_free_memory;
  }
  
  fprintf(stderr, "Press enter to lock\n");
  // ------------------------------------------------------------
  // Send and receive data
  
  write( fd_o, &arr1[0], n_byte);
  xdma2gpu(fd_i, dptr, n_byte);

  cuMemcpyDtoH(&arr2[0], (unsigned long long)dptr, n_byte);
  for (int i = 0; i < N; i++){
    if (arr1[i] != arr2[i])
      printf("%d -> %d\n", arr1[i], arr2[i]);
  }
  
  // ------------------------------------------------------------
  // Clean up
  
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
