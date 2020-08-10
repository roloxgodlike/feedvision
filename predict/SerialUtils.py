# !/usr/bin/env python3
# -*- coding: utf-8 -*-
import serial
import serial.tools.list_ports
import time
import re
import sys
import os

def show_list():
    plist = list(serial.tools.list_ports.comports())
    port_len = len(plist)
    if port_len == 0:
        print("serial port empty!")
        return port_len
    else:
        for port in plist:
            print("serial port info", port)
            # print("port", port[0])
            # print("device", port.device)
            # print("-----------------")
        return port_len

def open_port_default(port, baud, readTimeout):
    try:
        ser = serial.Serial(port, baud, timeout=readTimeout)
        print("serial port open: ", ser.isOpen(), port, baud, readTimeout)
        return ser
    except Exception as e:
        print('serial port open failed:', e)
        return None


MAX_LOOP_NUM = 10
def wait_response():
    maxloopNum = 0
    while True:
        line = ser.readline()
        maxloopNum = maxloopNum + 1
        try:
            print("Rsponse : %s" %line.decode('utf-8'))

        except:
            print('ex:', e)
        if ( re.search(b'OK',line)):
            break
        elif(maxloopNum > MAX_LOOP_NUM):
            sys.exit(0)

# while True:
#     time.sleep(1)
#     if ser.isOpen() == False:
#         ser.open()
#     try:
#         s = input('请输入一个字节:')
#         rxByte = bytes(s, encoding = "utf8")
#         # a_bytes = bytes.fromhex(a)
#         print("input:0x", rxByte.hex())
#         ser.write(rxByte)
#         print("Tx:0x", rxByte.hex())
#         rxByte = ser.read().hex()
#         print("Rx:0x", rxByte)
#         pass
#     except Exception as e:
#         print("ex:", e)
#         break

#     ser.close()
#     print("port close, isOpen:", ser.isOpen())