#!/bin/bash

read -p "Please input (Y/N): " yn

if [ "$yn" == "" ]; then
    echo "no input."
    exit 1
elif [ "$yn" == "y" ] || [ "$yn" == "Y" ]; then
    echo "OK, continue "
    exit 0
elif [ "$yn" == "n" ] || [ "$yn" == "N" ]; then
    echo "OK, interrupt "
    exit 0
else
    echo "I don't known what your choice is"
    exit 2
fi
