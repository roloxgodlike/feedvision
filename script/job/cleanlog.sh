#!/bin/bash

find /root/feedvision/ -name '*.log' > tmp.txt
cat tmp.txt | while read line
do
    echo "" > $line
done
rm -rf tmp.txt
