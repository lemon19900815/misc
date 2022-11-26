#!/bin/bash

echo "the script name is ==> $0"
echo "total parameter number is ==> $#"

[ "$#" -lt 2 ] && echo "the number of parameter is less than 2. stop here." && exit 1

echo "the whole parameter is ==> '$@'"
echo "the 1st parameter ==> $1"
echo "the 2nd parameter ==> $2"

shift # 进行一次“一个变量得shift”
echo "after shift..."
echo "total parameter number is ==> $#"
echo "the whole parameter is ==> '$@'"
