#!/bin/bash

#查看串口驱动
cat /proc/tty/drivers/serial

#查看串口设备
dmesg | grep ttyS*

#查看串口
ls -l /dev/ttyS*

while true
do
  echo 1 > /dev/ttyS0
  sleep 0.5
done

