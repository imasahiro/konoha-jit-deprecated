#!/bin/sh
make && sudo make install
konoha ../script/send.k script.k  || exit 1
#sudo cat /dev/script
