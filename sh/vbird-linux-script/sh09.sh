#!/bin/bash

# 1.告诉用户程序的用途，输入格式
echo "this program will try to calculate:"
echo "how many days before your demobilization date..."
read -p "please input your demobilization date (YYYYMMDD ex>20221120): " date2

# 2.利用正则表达式测试一下这个输入的内容是否正确
date_d=$(echo $date2 | grep '[0-9]\{8\}') # check输入是否8个数字
if [ "$date_d" == "" ]; then
    echo "you input the wrong date format..."
    exit 1
fi

# 3.开始计算日期
# declare -i date_tem=`date --date="$date_d" +%s`
# declare -i date_now=`date +%s`
# declare -i date_total_sec=$(($date_tem-$date_now))
# declare -i date_d=$(($date_total_sec/60/60/24))

date_tem=`date --date="$date_d" +%s`
date_now=`date +%s`
date_total_sec=$(($date_tem-$date_now))
date_d=$(($date_total_sec/60/60/24))

if [ "$date_d" -lt "0" ]; then
    echo "you have demobilization before: $((-1*$date_d)) days ago"
else
    echo "you will demobilization after $date_d days."
fi
