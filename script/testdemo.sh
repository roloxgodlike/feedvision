#!/bin/bash

BASE_PATH=/root/feedvision
IMG_PATH=$BASE_PATH/image/demo

isDebug=1
address=192.168.31.2
port=8000
username=admin
password=PassWord123
interval=10
picQuality=0
picSize=255

### shutdown all ###
/root/feedvision/script/shutdown.sh

### start all ###
#cd $BASE_PATH/HCNetCamera/lib
#nohup ./HCNetCamera \
#    --address=$address \
#    --port=$port \
#    --username=$username \
#    --password=$password \
#    --time-interval=$interval \
#    --dir=$IMG_PATH \
#    --pic-quality=$picQuality \
#    --pic-size=$picSize >/dev/null 2>&1 &
#echo "--- HCNetCamera started ---"


cd $BASE_PATH/predict
/usr/local/bin/python3.7 -u Predict.py $IMG_PATH $isDebug
#/usr/local/bin/python3.7 -u Predict.py $IMG_PATH $isDebug
echo "--- Predict.py started ---"
