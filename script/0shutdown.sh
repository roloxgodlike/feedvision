#!/bin/bash

PID=`ps -ef | grep $1 | grep -v grep | grep -v 'bash' | awk '{print $2}'`
if [ -n "$PID" ]
then
    echo "----- kill - pid: $PID - $1 -----"
    kill "$PID"
    sleep 1
    PID=`ps -ef | grep $1 | grep -v grep | grep -v 'bash' | awk '{print $2}'`
    if [ -n "$PID" ]
    then
        echo "----- force kill - pid: $PID - $1 -----"
    else
        echo "----- kill successfully - $1 -----"
    fi
else
    echo "----- $1 doesn't start -----"
fi
