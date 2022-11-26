#!/bin/bash

echo -e "I will use 'touch' command to create 3 files."
read -p "Please input your filename: " fileuser

filename=${fileuser:-"filename"}
if [ $filename = "filename" ]
then
    echo "no input filename."
    exit 1
fi

date1=$(date --date='2 days ago' +%Y%m%d)
date2=$(date --date='1 days ago' +%Y%m%d)
date3=$(date +%Y%m%d)
f1=${filename}${date1}".txt"
f2=${filename}${date2}".txt"
f3=${filename}${date3}".txt"

touch $f1 $f2 $f3
