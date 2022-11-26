#!/bin/bash

for animal in dog cat elephant
do
    echo "there are ${animal}s..."
done

users=`cut -d':' -f1 /etc/passwd`
for user in $users
do
    # id $user
    echo "$user: " `id $user` # 不能使用echo "$user: " id $user，否则后面不会被执行
    # finger $user # ubuntu没有这个命令
done
