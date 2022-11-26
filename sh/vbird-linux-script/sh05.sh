#!/bin/bash

read -p "Please input filename: " filename
test -z $filename && echo "You must input a filename." && exit 1
test ! -e $filename && echo "not exist $filename" && exit 1
test -f $filename && echo "$filename is regular file."
test -d $filename && echo "$filename is directory."


test -r $filename && perm="r" || perm='-'
test -w $filename && perm=$perm"w" || perm=$perm"-"
test -x $filename && perm=$perm"x" || perm=$perm"-"
echo "the permissions are: $perm"
