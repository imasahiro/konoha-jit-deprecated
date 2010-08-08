# this Makefile is generated from ../gensrc/gen_automake.k
# Copyright (c) 2010       Masahiro Ide <imasahiro9@gmail.com>
# shinpei@ynu(c)2008-09

# make target for libtool.
AM_CFLAGS = -g -pipe
lib_LTLIBRARIES = libkonoha.la

libkonoha_la_LDFLAGS = -no-undefined -release 0 -version-info 7:0:0
#if LDL
#libkonoha_la_LDFLAGS += -ldl
#endif

libkonoha_la_CFLAGS  = $(AM_CFLAGS) -O2 -Wall -Wformat -Wundef \
-Wreturn-type -Wpointer-arith 
AM_CPPFLAGS = -I../include
#if MINGW
#AM_CPPFLAGS += -DKONOHA_ON_MINGW
#endif
libkonoha_la_SOURCES = 
libkonoha_la_SOURCES += ../src/ext/mt19937-64.c
libkonoha_la_SOURCES += ../src/ext/mt19937ar.c
libkonoha_la_SOURCES += ../src/ext/qsort.c
libkonoha_la_SOURCES += ../src/ext/strerror.c
libkonoha_la_SOURCES += ../src/ext/strlen.c
libkonoha_la_SOURCES += ../src/lang/asm.c
libkonoha_la_SOURCES += ../src/lang/optimizer.c
libkonoha_la_SOURCES += ../src/lang/script.c
libkonoha_la_SOURCES += ../src/lang/ssa.c
libkonoha_la_SOURCES += ../src/lang/term.c
libkonoha_la_SOURCES += ../src/lang/typing.c
libkonoha_la_SOURCES += ../src/lang/vmcodelibs.c
libkonoha_la_SOURCES += ../src/lang/vminline.c
libkonoha_la_SOURCES += ../src/main/apidata/struct.c
libkonoha_la_SOURCES += ../src/main/array.c
libkonoha_la_SOURCES += ../src/main/bytes.c
libkonoha_la_SOURCES += ../src/main/class.c
libkonoha_la_SOURCES += ../src/main/closure.c
libkonoha_la_SOURCES += ../src/main/context.c
libkonoha_la_SOURCES += ../src/main/evidence.c
libkonoha_la_SOURCES += ../src/main/exports.c
libkonoha_la_SOURCES += ../src/main/iterator.c
libkonoha_la_SOURCES += ../src/main/map.c
libkonoha_la_SOURCES += ../src/main/memory.c
libkonoha_la_SOURCES += ../src/main/method.c
libkonoha_la_SOURCES += ../src/main/number.c
libkonoha_la_SOURCES += ../src/main/object.c
libkonoha_la_SOURCES += ../src/main/os.c
libkonoha_la_SOURCES += ../src/main/query.c
libkonoha_la_SOURCES += ../src/main/runtime.c
libkonoha_la_SOURCES += ../src/main/security.c
libkonoha_la_SOURCES += ../src/main/semantics.c
libkonoha_la_SOURCES += ../src/main/setjmp.c
libkonoha_la_SOURCES += ../src/main/stack.c
libkonoha_la_SOURCES += ../src/main/stream.c
libkonoha_la_SOURCES += ../src/main/string.c
libkonoha_la_SOURCES += ../src/main/system.c
libkonoha_la_SOURCES += ../src/main/thread.c
libkonoha_la_SOURCES += ../src/main/time.c
libkonoha_la_SOURCES += ../src/main/translator.c
libkonoha_la_SOURCES += ../src/main/tuple.c
libkonoha_la_includedir = $(includedir)
libkonoha_la_include_HEADERS = ../include/konoha.h
libkonoha_la_includekonohadir = $(includedir)/../include/konoha
libkonoha_la_includekonoha_HEADERS =
libkonoha_la_includekonoha_HEADERS += ../include/konoha/inlinelibs.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_api.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_class.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_code_.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_config.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_debug.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_deps.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_gc.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_glue.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_grammar_.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_lkm.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_name.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_stmt_.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_t.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_tb.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/konoha_vm.h
libkonoha_la_includekonoha_HEADERS += ../include/konoha/license.h
sampledir = $(prefix)/konoha/sample
dist_sample_SCRIPTS =
dist_sample_SCRIPTS += ../dist/sample/ao.k
dist_sample_SCRIPTS += ../dist/sample/counter.k
dist_sample_SCRIPTS += ../dist/sample/err.k
dist_sample_SCRIPTS += ../dist/sample/fib.k
dist_sample_SCRIPTS += ../dist/sample/helloworld.k
dist_sample_SCRIPTS += ../dist/sample/http.k
dist_sample_SCRIPTS += ../dist/sample/konohaget.k
dist_sample_SCRIPTS += ../dist/sample/main.k
dist_sample_SCRIPTS += ../dist/sample/readfile.k
dist_sample_SCRIPTS += ../dist/sample/sort.k
scriptklipdir = $(prefix)/konoha/script/klip
dist_scriptklip_SCRIPTS = ../dist/bin/klip.k
klipdir = $(bindir)
dist_klip_SCRIPTS = ../dist/bin/klip

man_MANS = ../konoha.1

bin_PROGRAMS = konoha
konoha_SOURCES = ../src/konoha.c
konoha_INCLUDES = ../include
konoha_LDADD = libkonoha.la

