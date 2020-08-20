#!/bin/bash

SCRIPT_PATH=/root/feedvision/script

count=`ps -ef | grep Predict.py | grep -v grep | wc -l`
if [ 0 -eq $count ]
then
    $SCRIPT_PATH/start.sh
else
    count=`ps -ef | grep HCNetCamera | grep -v grep | wc -l`
    if [ 0 -eq $count ] 
    then
        $SCRIPT_PATH/start.sh
    fi
fi

