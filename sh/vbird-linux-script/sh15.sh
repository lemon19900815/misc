#!/bin/bash

# use ping command to check the network's PC state.

network='192.168.3'
for ip in $(seq 1 10)
do
    # 检查ping的回传值是正确还是失败
    ping -c 1 -w 1 ${network}.${ip} &>/dev/null && result=1 || result=0
    if [ "$result" == "1" ]; then
        echo "Server ${network}.${ip} is UP."
    else
        echo "Server ${network}.${ip} is DOWN."
    fi
done
