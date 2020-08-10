#!/bin/bash

address=112.17.120.80
port=40001
username=admin
password=PassWord123
interval=10
dir=/dongjun/feedvision/img/target

cd lib
./HCNetCamera \
    --address=$address \
    --port=$port \
    --username=$username \
    --password=$password \
    --time-interval=$interval \
    --dir=$dir \
    --pic-quality=0 \
    --pic-size=255
