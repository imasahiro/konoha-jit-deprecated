
# define extra dirs
# these dirs only included for distribution archive.

EXTRA_SUBDIRS = src include dist
EXTRA_FILES = bootstrap konoha.1
# dirs which include Makefile.in
SUBDIRS = build 
EXTRA_DIST = $(EXTRA_SUBDIRS) $(EXTRA_FILES)

debug:
	-rm -f ./debug/
	cp -rf ./src/  ./debug
	$(CC) -O0 -Wall -g3  ./debug/*/*.c ./debug/konoha.c \
		-DHAVE_CONFIG_H -I. -I.. -I./include -o ./debug/konoha \
	$(AM_LDFLAGS) $(LDFLAGS) $(LIBS)

test: ./build/konoha
	konoha ./test/run.k ./build/konoha ./test/
