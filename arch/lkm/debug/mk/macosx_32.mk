# Last modified by Masahiro Ide imasahiro@sourceforge.jp

CC = gcc
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 -fPIC 
LDLIBS = -lkonoha

target = macosx_32
pkgname = debug

library = "$(pkgname)_$(target).dylib"

.PHONY: all
all: $(library)

objs = "$(pkgname).o" cmd.o kernel.o

"$(pkgname).o": $(pkgname).c
	$(CC) $(CFLAGS) -o $@ -c $^

"cmd.o": cmd.c
	$(CC) $(CFLAGS) -o $@ -c $^

"kernel.o": kernel.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(library): $(objs)
	$(CC) -dynamiclib -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(library)

