#!/bin/sh
sudo dmesg -c
make && sudo make install
konoha script/send.k script/device.k  || exit 1
sudo rm -f /dev/script
sudo mknod /dev/script c `konoha ./script/awk.k` 0 || exit 1
sudo cat /dev/script
cd ./debug/
sh ./install.sh
