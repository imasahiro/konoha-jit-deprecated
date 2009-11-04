
# define extra dirs
# these dirs only included for distribution archive.

EXTRA_SUBDIRS = src include dist
EXTRA_FILES = bootstrap konoha.1
# dirs which include Makefile.in
SUBDIRS = build 
EXTRA_DIST = $(EXTRA_SUBDIRS) $(EXTRA_FILES)

debug:
	-rm -f ./Debug/
	cp -rf ./src/  ./Debug
	$(CC) -O0 -Wall -g3  ./Debug/*/*.c ./Debug/konoha.c \
		-DHAVE_CONFIG_H -I. -I.. -I./include -o ./Debug/konoha \
	$(AM_LDFLAGS) $(LDFLAGS) $(LIBS)


