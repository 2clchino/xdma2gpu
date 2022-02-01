#include <chrono>
#include <iostream>

#include <stdio.h>
#include <unistd.h> // read(), open(), close()
#include <fcntl.h>
#include <stdlib.h> // posix_memalign()

int main(){
  static const int bufsize= 512*1024*1024;
  static const int buf_offset = 13;
  int msg_len = 32*1024*1024;  // must be even
  int min_size = 16;
  int max_iter = 100;
  
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
  
  int *txbuf, *rxbuf;
  int rv;

  rv = posix_memalign((void**)&txbuf, 4096, bufsize);
  if (rv != 0){
    printf("Can't allocate txbuf\n");
    return -1;
  }

  rv = posix_memalign((void**)&rxbuf, 4096, bufsize);
  if (rv != 0){
    printf("Can't allocate rxbuf\n");
    return -1;
  }

  printf("alloced.\n");

  // ------------------------------------------------------------
  // Fill buffer, send, receive and check.

  for (int i=0; i<msg_len; i++){
    txbuf[i+buf_offset] = i; // %256;
  }

  for (int size=min_size; size<=bufsize/4; size*=2){
    int n_iter = (bufsize/4)/size;
    if (n_iter>max_iter) n_iter = max_iter;
    //std::cout << "size" << size << "bufsize" << bufsize << "n_iter" << n_iter << "\n";
    auto start = std::chrono::system_clock::now();

    for (int i=0; i<n_iter; i++){
      write(fd_o, &txbuf[buf_offset], size*4);
      read (fd_i, &rxbuf[buf_offset], size*4);
    }
    auto stop = std::chrono::system_clock::now();
    
    auto dur = (stop-start);
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    
    std::cout << size << " " << msec << " ms. "
              << (double)size*4000.0f*msec/(n_iter) << " B/s\n";
  }
  
  // for (int i=0; i<msg_len; i++){
  //   int t = txbuf[i+buf_offset];
  //   int r = rxbuf[i+buf_offset];

  //   if (t!=r)
  //     printf("[%3d] %3d %3d\n", i, t, r);
  // }
  
  printf("done.\n");


  // ------------------------------------------------------------
  // Clean up
  
  free(txbuf);
  free(rxbuf);
  
  close(fd_o);
  close(fd_i);
}
