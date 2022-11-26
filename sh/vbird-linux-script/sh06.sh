#!/bin/bash

read -p "Please input (Y/N): " yn
[ "$yn" == "" ] && echo "no input." && exit 1
[ "$yn" == "y" -o "$yn" == "Y" ] && echo "OK, continue " && exit 0
[ "$yn" == "n" -o "$yn" == "N" ] && echo "OK, interrupt " && exit 0
echo "I don't known what your choice is" && exit 2
