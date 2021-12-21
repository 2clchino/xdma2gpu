#include "cuda.h"
#include "../xdma/cdev_sgdma.h"

#include <stdio.h>
#include <unistd.h> // read(), open(), close()
#include <fcntl.h>
#include <stdlib.h> // posix_memalign()
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <pthread.h>

// #define IOCTL_XDMA_WRITE          _IOR('q', 8, struct xdma_data_ioctl *)

typedef struct xdma_data_ioctl{
  char *value;
  size_t count;
} dma_read;

typedef struct parallel_write_arg{
  int fd;
  unsigned int cmd;
  dma_read *data;
} parallel_write;

typedef struct parallel_read_arg{
  int fd;
  char *data;
  size_t size;
} parallel_read;

void* write_test(void* p){
  parallel_write* datas;
  datas = (parallel_write*)p;
  printf("write: %s\n", datas->data->value);
  ioctl(datas->fd, datas->cmd, datas->data);
  return NULL;
}

void* read_test(void* p){
  parallel_read* datas;
  datas = (parallel_read*)p;
  printf("read: %s\n", datas->data);
  read(datas->fd, datas->data, datas->size);
  return NULL;
}

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

  // ------------------------------------------------------------
  // Send and receive data

  char hoge[20];
  char *addr;
  //pthread_t thr1, thr2;

  strcpy(hoge, "Hello");
  addr = &(hoge[0]);
  dma_read tmp = { addr, sizeof(hoge)};
  ioctl(fd_i, IOCTL_XDMA_GPU, &tmp);
  /*
  parallel_write write_data = { fd_o, IOCTL_XDMA_WRITE, &tmp};
  parallel_read read_data = { fd_i, addr, sizeof(hoge)};
  pthread_create( &thr1, NULL, write_test, (void*)(&write_data));
  pthread_create( &thr2, NULL, read_test, (void*)(&read_data));
  printf("main :%s\n", write_data.data->value);
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);
  pthread_detach(thr1);
  pthread_detach(thr2);
  */
  
  printf("%s\n", tmp.value);



  // ------------------------------------------------------------
  // Clean up
  
  close(fd_o);
  close(fd_i);
}
