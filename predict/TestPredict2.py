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
import numpy as np
warnings.filterwarnings("ignore")

clf0 = joblib.load('feedvision/predict/lr.model')
path = 'feedvision/image/demo' #要测试的图片路径

for img in os.listdir(path):
    image_path = os.path.join(path, img)
    image = cv2.imread(image_path)
    image = image[610:680,835:1130,]
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    num_pixels = image.shape[0] * image.shape[1]
    image_data = image.reshape((1, num_pixels))
    print(img, clf0.predict(image_data)[0])

img_array = []
for img in os.listdir(path):
    img_array = []
    image_path = os.path.join(path, img)
    image = cv2.imread(image_path)
    image = image[610:680,835:1130,]
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    img_array.append(image)
    img_array = np.array(img_array)
    num_pixels = img_array[0].shape[0] * img_array[0].shape[1]
    img_array = img_array.reshape((img_array.shape[0], num_pixels))
    results = clf0.predict(img_array)
    print(img, results[0])
