#!/bin/sh
cd ../gpuctl/ && make
sudo rmmod xdma.ko
sudo rmmod gpuctl.ko
sudo insmod gpuctl.ko
cd ../xdma/ && make
sudo insmod xdma.ko
cd ../usr/
