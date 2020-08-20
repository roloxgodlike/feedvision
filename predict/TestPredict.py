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

IMAGE_PATH_ROOT = 'feedvision/image/demo'
IS_DEBUG = 1
DEVICE_KEY = cUtils.get_mac_addr()
print('image_path: %s, device_key: %s, is_debug: %d' %(IMAGE_PATH_ROOT, DEVICE_KEY, IS_DEBUG))

clf1 = joblib.load('feedvision/predict/lr.model')
# clf1 = joblib.load('lr.model')
filelist = []
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
            image = image[610:680,835:1130,]
            image = cv2.resize(image, (256,256), interpolation=cv2.INTER_CUBIC)
            hist = cv2.calcHist([image], [0,1], None, [256,256], [0.0,255.0,0.0,255.0])
            result = clf1.predict([(hist/255).flatten()])
            if result == 1:
                print('1-exist')
            else:
                print('0-none')
            time.sleep(1)

    except Exception as e:
        print('ex:', e)
    finally:
        time.sleep(1)
    print('---------------------------------------------')
