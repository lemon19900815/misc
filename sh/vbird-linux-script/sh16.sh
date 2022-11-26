#!/bin/bash

# 1.先检查输入的目录是否存在
read -p "please input a directory: " dir
if [ "$dir" == "" -o ! -d "$dir" ]; then
    echo "the $dir is not exist in your system."
    exit 1
fi

# 2.开始测试文件属性
filelist=$(ls $dir)
for filename in $filelist
do
    perm=""
    test -r "$dir/$filename" && perm="${perm}r" || perm="${perm}-"
    test -w "$dir/$filename" && perm="${perm}w" || perm="${perm}-"
    test -x "$dir/$filename" && perm="${perm}x" || perm="${perm}-"
    echo "the file $dir/$filename's permission is $perm"
done

