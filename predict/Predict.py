# !/usr/bin/env python3
# -*- coding: utf-8 -*-
import cv2
import sys
import os
import warnings
import time
import random
import CommonUtils as cUtils
import SerialUtils as sUtils
import io
import joblib
warnings.filterwarnings("ignore")

IMAGE_PATH_ROOT = sys.argv[1]
# IMAGE_PATH_ROOT = 'feedvision/image/demo'
IS_DEBUG = int(sys.argv[2])
# IS_DEBUG = 1
DEVICE_KEY = cUtils.get_mac_addr()
API_URL = 'https://mes.poseidong.com/smart/device/access/alarm/data/push'
SERIAL_PORT = '/dev/ttyS0'
# SERIAL_PORT = 'COM4'
SERIAL_BAUD = 9600
SERIAL_TIMEOUT = 3

print('image_path: %s, device_key: %s, is_debug: %d' %(IMAGE_PATH_ROOT, DEVICE_KEY, IS_DEBUG))
print('api:', API_URL)
print('serial:', SERIAL_PORT, SERIAL_BAUD, SERIAL_TIMEOUT)

# clf1 = joblib.load('feedvision/predict/lr.model')
clf1 = joblib.load('lr.model')
filelist = []
alarm = {
    'init': 1,
    'flag': 0,
    'count': 0,
    'upload': 0,
    'batchid': '',
    'image_path': '' ,
    'is_changed': 0
}

port_len = sUtils.show_list()
if not port_len:
    exit()
SERIAL_PORT = sUtils.open_port_default(SERIAL_PORT, SERIAL_BAUD, SERIAL_TIMEOUT)
if not SERIAL_PORT:
    exit()

def reset_alarm_status():
    alarm['flag'] = 1 - alarm['flag']
    alarm['count'] = 0
    # alarm['upload'] = 0
    alarm['init'] = 0
    alarm['is_changed'] = 1

def process_alarm():
    if not alarm['flag']:
        # print('>>> alarm')
        reset_alarm_status()
    alarm['count'] += 1
    if alarm['is_changed'] and alarm['count'] == 5: #模型不准时的意外数据防抖
        alarm['upload'] = 0
        alarm['is_changed'] = 0
    if alarm['count'] < 6:
        return
    else:
        if not alarm['upload']:
            print('trigger alarm')
            handle_alarm()
        else:
            alarm['count'] = 0

def process_disalarm():
    if alarm['flag']:
        # print('>>> dis-alarm')
        reset_alarm_status()
    if alarm['init']: #首次如果正常状态不得累计
        return
    alarm['count'] += 1
    if alarm['is_changed'] and alarm['count'] == 5:
        alarm['upload'] = 0
        alarm['is_changed'] = 0
    if alarm['count'] < 6:
        return
    else:
        if not alarm['upload']:
            print('trigger dis-alarm')
            handle_disalarm()
        else:
            alarm['count'] = 0

def get_alarm_batchid(tt):
    return '{}{}'.format(tt, random.randint(1000000,9999999))

def handle_alarm():
    cmd = b'\xFE\x05\x00\x00\xFF\x00\x98\x35' #alarm
    SERIAL_PORT.write(cmd)
    alarm_upload(True)

def handle_disalarm():
    cmd = b'\xFE\x05\x00\x00\x00\x00\xD9\xC5' #disalarm
    SERIAL_PORT.write(cmd)
    alarm_upload(False)

def alarm_upload(is_alarm):
    tt = cUtils.get_timestamp_mills()

    batchid = ''
    if is_alarm:
        batchid = get_alarm_batchid(tt)
        alarm['batchid'] = batchid
    else:
        batchid = alarm['batchid']

    data = {
        'alarmBatchId': batchid,
        'alarmTime': tt,
        'alarmType': '1',
        'deviceKey': DEVICE_KEY,
        'imgData': cUtils.img_to_base64(process_img_path)
    }

    if IS_DEBUG:
        del data['imgData']
        print('http-tx:', data)
        alarm['upload'] = 1
    else:
        respJson = cUtils.http_post_json(API_URL, data)
        del data['imgData']
        print('http-tx:', data)
        print('http-rx:', respJson)
        if respJson is None or respJson['success'] == False:
            print('upload failed')
        else:
            alarm['upload'] = 1

while 1:

    filelist.clear()
    filelist = os.listdir(IMAGE_PATH_ROOT)
    filelen = len(filelist)
    print('image len:', filelen)
    if filelen == 0: # avoid file empty
        time.sleep(1)
        continue

    try:
        # read and analyse image
        for img in filelist:
            image_path = os.path.join(IMAGE_PATH_ROOT, img)
            process_img_path = image_path
            image = cv2.imread(image_path)
            # image = image[370:480,720:1030,]
            image = image[620:675,840:1065,]
            image = cv2.resize(image, (256,256), interpolation=cv2.INTER_CUBIC)
            hist = cv2.calcHist([image], [0,1], None, [256,256], [0.0,255.0,0.0,255.0])
            result = clf1.predict([(hist/255).flatten()])
            if result == 1:
                print('1-exist')
                process_disalarm()
            else:
                print('0-none')
                process_alarm()
            if IS_DEBUG:
                time.sleep(1)
            else:
                os.remove(image_path)


    except Exception as e:
        print('ex:', e)
    finally:
        filelist.clear()
        filelist = os.listdir(IMAGE_PATH_ROOT)
        print('image remain:', len(filelist))
        if IS_DEBUG:
            time.sleep(1)
        else:
            for img in filelist:
                os.remove(os.path.join(IMAGE_PATH_ROOT,img))
            time.sleep(10)
        print('---------------------------------------------')
