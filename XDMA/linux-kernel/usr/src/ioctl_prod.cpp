#include "ioctl_gpu.h"
# define N 32 * 1024 * 1024
int main(){
  int fd_o = open("/dev/xdma0_h2c_0", O_WRONLY);
  int fd_i = open("/dev/xdma0_c2h_0", O_RDONLY);
  
  checkError(cuInit(0));
  CUdevice device;
  checkError(cuDeviceGet(&device, 0));
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
  cuMemAlloc(&dptr, n_byte);
  cuPointerSetAttribute(&flag,
		    CU_POINTER_ATTRIBUTE_SYNC_MEMOPS, dptr);
  
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
  cuMemFree(dptr);
  cuCtxDestroy(context);
  close(fd_o);
  close(fd_i);
  return 0;
}
