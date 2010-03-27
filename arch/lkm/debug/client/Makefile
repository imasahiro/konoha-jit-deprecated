# Last modified by Masahiro Ide imasahiro@users.sourceforge.jp

CC = gcc
CFLAGS = -g3 -O0 -Wall 
LDLIBS = -lkonoha

target = linux_32
pkgname = kdb

.PHONY: all
all: $(pkgname) 

$(pkgname): $(pkgname).c 
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	$(RM) -rf $(pkgname)

