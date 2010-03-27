#!/bin/sh
cd ./debug/
sh ./uninstall.sh
cd ../
echo "c.unregister();" > /dev/konoha
sudo rmmod konohadev
