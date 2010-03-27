#!/usr/local/bin/konoha
o = new OutputStream("Makefile", "w");
o << """
#
# Makefile for the linux ramfs routines.
#
KVER = $(shell uname -r)
KDIR = /lib/modules/$(KVER)/build
KBUILD_AFLAGS += -DKONOHA_ON_LKM
KBUILD_CFLAGS += -DKONOHA_ON_LKM
KBUILD_CFLAGS += -Wno-declaration-after-statement
VERBOSE=0
""" << EOL;

o.println(%("KBUILD_CFLAGS += -I%s{0}/../../../include/",$env.PWD));
o.println(%("KBUILD_CFLAGS += -I%s{0}/../",$env.PWD));

o << """
ifneq ($(KERNELRELEASE),)
obj-m := kfs.o
kfs-objs := super.o dir.o file.o
else
default:modules

install-kfs:
	/sbin/insmod ./kfs.ko  || exit 1

install:install-kfs
#	chgrp $(shell whoami) /dev/konoha
#	chmod 666   /dev/konoha

modules::
	ln -sf ../Module.symvers .
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean::
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean

endif
""" << EOL;
o.close();
