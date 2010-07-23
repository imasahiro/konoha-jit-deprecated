#!/bin/sh
# ./gencoverage.sh
# ** Please install "lcov" **
#
# $ cd /path/to/konoha/
# $ konoha ./gensrc/gencoverage.h
# $ open lcov/index.html

DIR=`pwd`
cd $DIR/src/ext/
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include mt19937-64.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include mt19937ar.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include qsort.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include strerror.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include strlen.c
cd $DIR/src/lang
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include asm.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include optimizer.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include script.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include ssa.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include term.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include typing.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include vmcodelibs.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include vminline.c
cd $DIR/src/main/apidata
gcc -O2 -Wall -g -coverage -DHAVE_CONFIG_H -I. -I.. -I./include struct.c -c 
cd $DIR/src/main 
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include array.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include bytes.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include class.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include closure.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include context.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include empty.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include evidence.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include exports.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include iterator.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include konohaapi.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include map.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include memory.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include method.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include number.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include object.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include os.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include query.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include security.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include semantics.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include stack.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include stream.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include string.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include system.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include thread.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include time.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include translator.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include tuple.c
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include updater.c
cd $DIR/src/
gcc -O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I../include konoha.c
cd $DIR/
gcc -O2 -Wall -g -coverage -DHAVE_CONFIG_H -I. -I.. -I./include \
$DIR/src/ext/mt19937-64.o      \
$DIR/src/ext/mt19937ar.o       \
$DIR/src/ext/qsort.o           \
$DIR/src/ext/strerror.o        \
$DIR/src/ext/strlen.o          \
$DIR/src/lang/asm.o            \
$DIR/src/lang/optimizer.o      \
$DIR/src/lang/script.o         \
$DIR/src/lang/ssa.o            \
$DIR/src/lang/term.o           \
$DIR/src/lang/typing.o         \
$DIR/src/lang/vmcodelibs.o     \
$DIR/src/lang/vminline.o       \
$DIR/src/main/apidata/struct.o \
$DIR/src/main/array.o          \
$DIR/src/main/bytes.o          \
$DIR/src/main/class.o          \
$DIR/src/main/closure.o        \
$DIR/src/main/context.o        \
$DIR/src/main/empty.o          \
$DIR/src/main/evidence.o       \
$DIR/src/main/exports.o        \
$DIR/src/main/iterator.o       \
$DIR/src/main/konohaapi.o      \
$DIR/src/main/map.o            \
$DIR/src/main/memory.o         \
$DIR/src/main/method.o         \
$DIR/src/main/number.o         \
$DIR/src/main/object.o         \
$DIR/src/main/os.o             \
$DIR/src/main/query.o          \
$DIR/src/main/security.o       \
$DIR/src/main/semantics.o      \
$DIR/src/main/stack.o          \
$DIR/src/main/stream.o         \
$DIR/src/main/string.o         \
$DIR/src/main/system.o         \
$DIR/src/main/thread.o         \
$DIR/src/main/time.o           \
$DIR/src/main/translator.o     \
$DIR/src/main/tuple.o          \
$DIR/src/main/updater.o        \
$DIR/src/konoha.o              \
-lsqlite3 -lpthread -ldl -lgcov -o ./konoha

./konoha -t \
$DIR/ktest/class/boolean.ktest       \
$DIR/ktest/class/float.ktest         \
$DIR/ktest/class/int.ktest           \
$DIR/ktest/class/stream.ktest        \
$DIR/ktest/statement/auxiliary.ktest \
$DIR/ktest/statement/case.ktest      \
$DIR/ktest/statement/for.ktest       \
$DIR/ktest/statement/if.ktest       \
$DIR/ktest/statement/while.ktest    \

./konoha $DIR/bench/microbench2.k

lcov -c -d . -o $DIR/konoha.info
genhtml -o $DIR/lcov -p $DIR --num-spaces 4  $DIR/konoha.info

rm -f \
$DIR/src/ext/mt19937-64.{o,gcno,gcda}      \
$DIR/src/ext/mt19937ar.{o,gcno,gcda}       \
$DIR/src/ext/qsort.{o,gcno,gcda}           \
$DIR/src/ext/strerror.{o,gcno,gcda}        \
$DIR/src/ext/strlen.{o,gcno,gcda}          \
$DIR/src/lang/asm.{o,gcno,gcda}            \
$DIR/src/lang/optimizer.{o,gcno,gcda}      \
$DIR/src/lang/script.{o,gcno,gcda}         \
$DIR/src/lang/ssa.{o,gcno,gcda}            \
$DIR/src/lang/term.{o,gcno,gcda}           \
$DIR/src/lang/typing.{o,gcno,gcda}         \
$DIR/src/lang/vmcodelibs.{o,gcno,gcda}     \
$DIR/src/lang/vminline.{o,gcno,gcda}       \
$DIR/src/main/apidata/struct.{o,gcno,gcda} \
$DIR/src/main/array.{o,gcno,gcda}          \
$DIR/src/main/bytes.{o,gcno,gcda}          \
$DIR/src/main/class.{o,gcno,gcda}          \
$DIR/src/main/closure.{o,gcno,gcda}        \
$DIR/src/main/context.{o,gcno,gcda}        \
$DIR/src/main/empty.{o,gcno,gcda}          \
$DIR/src/main/evidence.{o,gcno,gcda}       \
$DIR/src/main/exports.{o,gcno,gcda}        \
$DIR/src/main/iterator.{o,gcno,gcda}       \
$DIR/src/main/konohaapi.{o,gcno,gcda}      \
$DIR/src/main/map.{o,gcno,gcda}            \
$DIR/src/main/memory.{o,gcno,gcda}         \
$DIR/src/main/method.{o,gcno,gcda}         \
$DIR/src/main/number.{o,gcno,gcda}         \
$DIR/src/main/object.{o,gcno,gcda}         \
$DIR/src/main/os.{o,gcno,gcda}             \
$DIR/src/main/query.{o,gcno,gcda}          \
$DIR/src/main/security.{o,gcno,gcda}       \
$DIR/src/main/semantics.{o,gcno,gcda}      \
$DIR/src/main/stack.{o,gcno,gcda}          \
$DIR/src/main/stream.{o,gcno,gcda}         \
$DIR/src/main/string.{o,gcno,gcda}         \
$DIR/src/main/system.{o,gcno,gcda}         \
$DIR/src/main/thread.{o,gcno,gcda}         \
$DIR/src/main/time.{o,gcno,gcda}           \
$DIR/src/main/translator.{o,gcno,gcda}     \
$DIR/src/main/tuple.{o,gcno,gcda}          \
$DIR/src/main/updater.{o,gcno,gcda}        \
$DIR/src/konoha.{o,gcno,gcda}              \

