# !/usr/bin/env python3
# -*- coding: utf-8 -*-
import time

def get_timestamp_sec():
    return int(time.time())

def get_timestamp_mills():
    return int(time.time() * 1000)



import urllib.request
import urllib.parse
import json
import ssl
ssl._create_default_https_context = ssl._create_unverified_context

def http_post_json(url, data):
    try:
        headers = {'Content-Type': 'application/json'}
        data = json.dumps(data)
        dataEncode = data.encode('utf-8')
        dataEncode = data.encode('utf-8')
        request = urllib.request.Request(url, dataEncode, headers)
        response = urllib.request.urlopen(request, timeout=30)
        respData = response.read().decode('utf-8')
        respJson = json.loads(respData)
        return respJson
    except urllib.error.URLError as e:
        print('err:', e)
        return {'success':False}
    except Exception as e:
        print('ex:', e)
        return {'success':False}

def http_get_json(url):
    try:
        request = urllib.request.Request(url)
        response = urllib.request.urlopen(request)
        respData = response.read().decode('utf-8')
        respJson = json.loads(respData)
        return respJson
    except urllib.error.URLError as e:
        print('err:', e)
        return {'success':False}
    except Exception as e:
        print('ex:', e)
        return {'success':False}



import base64

def img_to_base64(imgPath):
    with open(imgPath,'rb') as f:
        b64 = base64.b64encode(f.read())
        return b64.decode()

def base64_to_Img(base64Str, targetPath):
    with open(targetPath, 'wb') as f:
        imgBytes = base64.b64decode(base64Str.encode())
        f.write(imgBytes)
    f.close()



import uuid

def get_mac_addr():
    node = uuid.getnode()
    mac = uuid.UUID(int = node).hex[-12:]
    return mac

# def get_mac_addr():
#     import sys
#     import os
#     mac = None
#     print(sys.platform)
#     if sys.platform == "win32":
#         start = 0
#         for line in os.popen("ipconfig/all"):
#             if line.lstrip().startswith("网卡名字"):
#                 start = 1
#             if start == 1 and line.lstrip().startswith("Physical Address"):
#                 mac = line.split(":")[1].strip().replace("-", ":")
#                 break
#     else:
#         for line in os.popen("/sbin/ifconfig"):
#             if 'Ether' in line:
#                 mac = line.split()[1]
#                 break
#     return mac