#!/bin/bash

if [ $(id -u) -ne 0 ]; then
    echo Please run this script as root or using sudo!
    exit 1
fi

lsmod | grep hello && rmmod hello_homework

insmod ./hello_homework.ko
if [ $? -ne 0 ]; then
    echo Fail 1
    exit 1
fi

stat /sys/module/hello_homework/parameters/str_param &>/dev/null
if [ $? -ne 0 ]; then
    echo Fail 2
    exit 1
fi

sysfs_dir="/sys/module/hello_homework/parameters"

test_str="Hello world!"

for ((i = 0; i < ${#test_str}; i++)); do
    echo "$i - ${test_str:$i:1}"
    echo "$i" >$sysfs_dir/idx_param
    echo "${test_str:$i:1}" >$sysfs_dir/chval_param
done

echo "Resulting value: "
cat $sysfs_dir/str_param

# rmmod hello_homework
