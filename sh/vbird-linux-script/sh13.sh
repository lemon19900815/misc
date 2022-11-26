#!/bin/bash

idx=0
# while [ "$idx" != "100" ]
# do
    # idx=$(($idx+1))
    # sum=$(($sum+$idx))
# done

until [ "$idx" == "100" ]
do
    idx=$(($idx+1))
    sum=$(($sum+$idx))
done

echo "sum = $sum"
