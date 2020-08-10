#!/bin/bash

BASE_PATH=/dongjun/feedvision
IMG_PATH=$BASE_PATH/img/target

address=112.17.120.80
port=40001
username=admin
password=PassWord123
interval=10
picQuality=0
picSize=255

### shutdown all ###
./shutdown.sh

### start all ###
cd $BASE_PATH/predict
python3 Predict.py $IMG_PATH >/dev/null 2>&1 &
echo "--- Predict.py started ---"

cd $BASE_PATH/HCNetCamera/lib
./HCNetCamera \
    --address=$address \
    --port=$port \
    --username=$username \
    --password=$password \
    --time-interval=$interval \
    --dir=$IMG_PATH \
    --pic-quality=$picQuality \
    --pic-size=$picSize >/dev/null 2>&1 &
echo "--- HCNetCamera started ---"

