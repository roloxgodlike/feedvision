# !/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import json
import time
import random
import CommonUtils as cUtils
import SerialUtils as sUtils
import serial

sUtils.show_list()
ser = sUtils.open_port_default("/dev/ttyS0", 9600, 3)

while 1:
  try:
    cmd = b'\xFE\x05\x00\x00\xFF\x00\x98\x35'  # 报警
    ser.write(cmd)
    time.sleep(5)
    cmd = b'\xFE\x05\x00\x00\x00\x00\xD9\xC5'  # 解除
    ser.write(cmd)
    time.sleep(5)
  except Exception as e:
    print('ex:', e)
    if ser.isOpen(): 
      ser.close()
    exit()
