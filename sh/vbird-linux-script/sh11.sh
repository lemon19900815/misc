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

case $1 in
  one)
    echo "your choice is One"
    ;;
  two)
    echo "your choice is Two"
    ;;
  three)
    echo "your choice is Three"
    ;;
  *)
    echo "Usage: $0 {one|two|three}"
    ;;
esac