#
# Makefile for the linux ramfs routines.
#
KVER = $(shell uname -r)
KDIR = /lib/modules/$(KVER)/build
KBUILD_AFLAGS += -DKONOHA_ON_LKM
KBUILD_CFLAGS += -DKONOHA_ON_LKM
KBUILD_CFLAGS += -Wno-declaration-after-statement
VERBOSE=0

KBUILD_CFLAGS += -I/home/tsunade/konoha/branches/konoha-cdev/arch/lkm/fs/../../../include/
KBUILD_CFLAGS += -I/home/tsunade/konoha/branches/konoha-cdev/arch/lkm/fs/../
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

