# !/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import json
import time
import random
import CommonUtils as cUtils
import SerialUtils as sUtils

# print(cUtils.getTimestampMills())

DEVICE_KEY = cUtils.get_mac_addr()
# API_URL = 'http://192.168.0.5:8550/smart/device/access/alarm/data/push'
# API_URL = 'https://mes.poseidong.com/smart/device/access/alarm/data/push'

def get_alarm_batchid(tt):
    return '{}{}'.format(tt, random.randint(1000000,9999999))

tt = cUtils.get_timestamp_mills()
batchid = get_alarm_batchid(tt)
b64 = cUtils.img_to_base64('../image/10187.jpg')
print(b64)
data = {
    'alarmBatchId': batchid,
    'alarmTime': tt,
    'alarmType': '1',
    'deviceKey': DEVICE_KEY,
    'imgData': b64
}
respJson = cUtils.http_post_json(API_URL, data)
print(respJson)
print(respJson['success'])
