#!/bin/bash
# ./gencoverage.sh
# ** Please install "lcov" **
#
# $ cd /path/to/konoha/
# $ bash ./gensrc/gencoverage.sh -R -C
# $ open lcov/index.html


CC=gcc
DIR=`pwd`
arch=`uname -a`;
if [ "$arch" = "Darwin" ]; then
    ver=`uname -r`;
    if [ "$ver" = "9.8.0" ]; then
        CC="$CC-4.2"
    fi
fi

C="false"
R="false"
for v in $* 
do
    if [ "$v" = "-C" ]; then
        C="true";
    fi
    if [ "$v" = "-R" ]; then
        R="true";
    fi
    if [ "$v" = "-h" ]; then
        echo "Usage: $0 [OPTIONS]"
        echo "  -h : Print this help, then exit"
        echo "  -C : Compile Source  Code before code coverage"
        echo "  -R : Remove coverage data after  code coverage"
        exit
    fi
done

if [ "$C" = "false" ]; then
    if [ ! -e src/konoha.gcno ]; then
        C="true"
    fi
fi

if [ "$C" = "true" ]; then
CFLAGS="-O2 -Wall -g -coverage -c -DHAVE_CONFIG_H -I. -I.. -I./include"
cd $DIR/src/ext/
$CC $CFLAGS mt19937-64.c
$CC $CFLAGS mt19937ar.c
$CC $CFLAGS qsort.c
$CC $CFLAGS strerror.c
$CC $CFLAGS strlen.c
cd $DIR/src/lang
$CC $CFLAGS asm.c
$CC $CFLAGS optimizer.c
$CC $CFLAGS script.c
$CC $CFLAGS ssa.c
$CC $CFLAGS term.c
$CC $CFLAGS typing.c
$CC $CFLAGS vmcodelibs.c
$CC $CFLAGS vminline.c
cd $DIR/src/main/apidata
$CC $CFLAGS struct.c 
cd $DIR/src/main 
$CC $CFLAGS array.c
$CC $CFLAGS bytes.c
$CC $CFLAGS class.c
$CC $CFLAGS closure.c
$CC $CFLAGS context.c
$CC $CFLAGS empty.c
$CC $CFLAGS evidence.c
$CC $CFLAGS exports.c
$CC $CFLAGS iterator.c
$CC $CFLAGS konohaapi.c
$CC $CFLAGS map.c
$CC $CFLAGS memory.c
$CC $CFLAGS method.c
$CC $CFLAGS number.c
$CC $CFLAGS object.c
$CC $CFLAGS os.c
$CC $CFLAGS query.c
$CC $CFLAGS security.c
$CC $CFLAGS semantics.c
$CC $CFLAGS stack.c
$CC $CFLAGS stream.c
$CC $CFLAGS string.c
$CC $CFLAGS system.c
$CC $CFLAGS thread.c
$CC $CFLAGS time.c
$CC $CFLAGS translator.c
$CC $CFLAGS tuple.c
$CC $CFLAGS updater.c
cd $DIR/src/
$CC $CFLAGS konoha.c
cd $DIR/
$CC -O2 -Wall -g -coverage -DHAVE_CONFIG_H -I. -I.. -I./include \
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
fi

lcov -z -d .

./konoha -t \
$DIR/ktest/class/array_F.ktest       \
$DIR/ktest/class/boolean.ktest       \
$DIR/ktest/class/float.ktest         \
$DIR/ktest/class/int.ktest           \
$DIR/ktest/class/stream.ktest        \
$DIR/ktest/class/system.ktest        \
$DIR/ktest/class/connection.ktest    \
$DIR/ktest/class/string.ktest        \
$DIR/ktest/statement/auxiliary.ktest \
$DIR/ktest/statement/for.ktest       \
$DIR/ktest/statement/if.ktest        \
$DIR/ktest/statement/while.ktest     \

./konoha -t \
$DIR/ktest/class/array_I.ktest

./konoha $DIR/bench/microbench2.k
./konoha $DIR/bench/stringbench.k
##./konoha $DIR/bench/gcbench.k

lcov -c -d . -o $DIR/konoha.info
genhtml -o $DIR/lcov -p $DIR --num-spaces 4  $DIR/konoha.info

if [ "$R" = "true" ]; then
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

fi
