#!/bin/bash

# read -p "input your choice: " choice
# case $choice in

# case $1 in
  # "one")
    # echo "your choice is One"
    # ;;
  # "two")
    # echo "your choice is Two"
    # ;;
  # "three")
    # echo "your choice is Three"
    # ;;
  # *)
    # echo "Usage: $0 {one|two|three}"
    # ;;
# esac

function printit() {
    # echo -n "your choice is " # -n可以不断行继续在同一行显示
    echo "your choice is " $1 # -n可以不断行继续在同一行显示
}

# case $1 in
  # one)
    # printit; echo $1 | tr 'a-z' 'A-Z' # 将参数做大小写转换
    # ;;
  # two)
    # printit; echo $1 | tr 'a-z' 'A-Z' # 将参数做大小写转换
    # ;;
  # three)
    # printit; echo $1 | tr 'a-z' 'A-Z' # 将参数做大小写转换
    # ;;
  # *)
    # echo "Usage: $0 {one|two|three}"
    # ;;
# esac

case $1 in
  one | two | three)
    printit $1
    ;;
  # two)
    # printit $1
    # ;;
  # three)
    # printit $1
    # ;;
  *)
    echo "Usage: $0 {one|two|three}"
    ;;
esac