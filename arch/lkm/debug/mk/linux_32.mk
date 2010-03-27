# Last modified by Masahiro Ide imasahiro@users.sourceforge.jp

CC = gcc
CFLAGS = -O2 -Wall -fmessage-length=0 -fPIC -g
LDLIBS = -lkonoha

target = linux_32
pkgname = debug

library = "$(pkgname)_$(target).so"

.PHONY: all
all: $(library)

objs = "$(pkgname).o" cmd.o

"$(pkgname).o": $(pkgname).c
	$(CC) $(CFLAGS) -o $@ -c $^

"cmd.o": cmd.c
	$(CC) $(CFLAGS) -o $@ -c $^


$(library): $(objs)
	$(CC) -shared -Wl -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(library)

