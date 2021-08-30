#include <stdio.h>
#include <unistd.h> // read(), open(), close()
#include <fcntl.h>
#include <stdlib.h> // posix_memalign()
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <string.h>

#define IOCTL_XDMA_WRITE          _IOR('q', 8, struct xdma_data_ioctl *)

typedef struct xdma_data_ioctl{
  char *value;
  size_t count;
} dma_read;

int main(){
  static const int bufsize=1024;
  static const int buf_offset = 13;
  static const int msg_len = 32;  // must be even

  

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
  // Buffer allocation
  
  char *txbuf, *rxbuf;
  char hoge[95];
  char *addr;
  int rv;

  rv = posix_memalign((void*)&txbuf, 4096, bufsize);
  if (rv != 0){
    printf("Can't allocate txbuf\n");
    return -1;
  }

  rv = posix_memalign((void*)&rxbuf, 4096, bufsize);
  if (rv != 0){
    printf("Can't allocate rxbuf\n");
    return -1;
  }

  // ------------------------------------------------------------  // Fill buffer, send, receive and check.

  for (int i=0; i<msg_len; i++){
    txbuf[i+buf_offset] = i%256;
  }
  strcpy(addr, "Hello");
  dma_read tmp = { addr, sizeof(hoge)};
  
  ioctl(fd_o, IOCTL_XDMA_WRITE, &tmp);
  read (fd_i, addr, sizeof(hoge));
  // unlocked_ioctl(fd_i, 0, 0);
  // read (fd_i, &rxbuf[buf_offset], msg_len);
  /*
  for (int i=0; i<msg_len; i++){
    printf("[%3d] %3d %3d\n", i, txbuf[i+buf_offset], rxbuf[i+buf_offset]);
  }
  */
  printf("%s\n", tmp.value);
  printf("%s\n", addr);



  // ------------------------------------------------------------
  // Clean up

  free(txbuf);
  free(rxbuf);

  close(fd_o);
  close(fd_i);
}
