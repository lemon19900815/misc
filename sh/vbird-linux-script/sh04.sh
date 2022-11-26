#!/bin/bash
# Program:

echo -e "input 2 number, I will cross them!"

read -p "first number: " first_no
read -p "second number: " second_no

total=$(($first_no*$second_no))
echo -e "$first_no x $second_no = $total"