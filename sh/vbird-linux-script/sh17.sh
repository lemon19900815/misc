#!/bin/bash

read -p "please input a number, i will count for 1+2+...+your_input: " num

# 注意：'(('括号之间没有空格，否则会报错

sum=0
for (( i=1; i<=$num; i++ ))
do
    sum=$(( $sum+$i )) # '((' -o '))'括号不分开
done

echo "the result of '1+2+...+$num' is ==> $sum"

# 可以使用 sh +x script.sh来进行程序的调试