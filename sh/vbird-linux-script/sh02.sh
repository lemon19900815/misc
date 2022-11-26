#!/bin/bash
# Program:
#	User inputs his first name and last name. Program shows his full name.
# History:
# 2022/11/15 lemon First release

read -p "Please input your first name: " firstname
read -p "Please input your last name: " lastname

echo -e "\nYour full name is: $firstname $lastname"
