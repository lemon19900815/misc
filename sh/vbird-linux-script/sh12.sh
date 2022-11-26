#!/bin/bash

# -a: 同and，2个条件同时成立
# -o: 同or，1个条件成立
# while [ "$yn" != "yes" -a "$yn" != "YES" ]
# do
    # read -p "please input yes/YES to stop this program: " yn
# done

until [ "$yn" == "yes" -o "$yn" == "YES" ]
do
    read -p "please input yes/YES to stop this program: " yn
done

echo "ok, you input the correct answer."
