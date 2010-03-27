#!/usr/local/bin/konoha
o = new OutputStream("Makefile", "w");
o << """
#
# Makefile for the linux ramfs routines.
#
KVER = $(shell uname -r)
KDIR = /lib/modules/$(KVER)/build
KBUILD_CFLAGS += -DKONOHA_ON_LKM
KBUILD_CFLAGS += -Wno-declaration-after-statement
VERBOSE=0
""" << EOL;

o.println(%("KBUILD_CFLAGS += -I%s{0}/../../../include/",$env.PWD));
o.println(%("KBUILD_CFLAGS += -I%s{0}/../",$env.PWD));

o << """
ifneq ($(KERNELRELEASE),)
obj-m := kdb.o
kdb-objs := debug.o cmd.o kernel.o
else
default:konoha modules

install-dbg:
	/sbin/insmod ./kdb.ko  || exit 1

install:install-dbg
	rm -f /dev/kdb
	mknod /dev/kdb c $(shell awk "\$$2==\\"kdb\\" {print \$$1}" /proc/devices) 0
	chgrp $(shell whoami) /dev/kdb
	chmod 666   /dev/kdb


konoha:
	$(MAKE) -C ../

modules::
	ln -sf ../Module.symvers .
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean::
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean

endif
""" << EOL;
o.close();
