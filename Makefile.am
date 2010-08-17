
# define extra dirs
# these dirs only included for distribution archive.

EXTRA_SUBDIRS = src include dist
EXTRA_FILES = bootstrap konoha.1
# dirs which include Makefile.in
SUBDIRS = build 
EXTRA_DIST = $(EXTRA_SUBDIRS) $(EXTRA_FILES)

test: ./build/konoha
	./build/konoha -t \
		./ktest/class/array_F.ktest       \
		./ktest/class/boolean.ktest       \
		./ktest/class/float.ktest         \
		./ktest/class/int.ktest           \
		./ktest/class/stream.ktest        \
		./ktest/class/system.ktest        \
		./ktest/class/connection.ktest    \
		./ktest/class/string.ktest        \
		./ktest/statement/auxiliary.ktest \
		./ktest/statement/for.ktest       \
		./ktest/statement/if.ktest        \
		./ktest/statement/while.ktest
	./build/konoha -t \
		./ktest/path/file.ktest           \
		./ktest/path/dir.ktest            \
		./ktest/path/lib.ktest            \
		./ktest/path/script.ktest         \
		./ktest/path/charset.ktest        \
		./ktest/path/class.ktest
	
	./build/konoha -t ./ktest/class/array_I.ktest
	./build/konoha -t ./ktest/class/array_S.ktest
	./build/konoha -t ./ktest/class/object.ktest

