# this Makefile is generated from ../gensrc/gen_nmake.k
# Copyright (c) 2010       Masahiro Ide <imasahiro9@gmail.com>
KONOHA=konoha
OUTDIR=.
CC=cl
LINK32=link.exe
DISTDIR="\Konoha"
BINDIR="\Konoha\bin"

ALL : $(OUTDIR)\$(KONOHA).dll $(OUTDIR)\$(KONOHA).exe

CLEAN :
	erase /Q $(OUTDIR)\*.obj

$(OUTDIR) :
	if not exist $(OUTDIR)/$(NULL) mkdir $(OUTDIR)

$(DISTDIR) :
	if not exist $(DISTDIR)/$(NULL) mkdir $(DISTDIR)

INSTALL : $(DISTDIR)
       -@erace /Q $(DISTDIR)\*
       if not exist $(DISTDIR)\bin\$(NULL) mkdir $(DISTDIR)\bin
       if not exist $(DISTDIR)\lib\$(NULL) mkdir $(DISTDIR)\lib
       if not exist $(DISTDIR)\package\$(NULL) mkdir $(DISTDIR)\package
       copy "$(OUTDIR)\konoha.exe" "$(DISTDIR)\bin\konoha.exe"
       copy "$(OUTDIR)\konoha.dll" "$(DISTDIR)\bin\konoha.dll"
       copy "$(OUTDIR)\konoha.lib" "$(DISTDIR)\lib\konoha.lib"

CFLAGS=/nologo /c /TP /GL /W3 /O2 /I"..\include" /I".\include"

CFLAGSD=/nologo /c /Od /Oy /W3 /MD /TP /Zi\
       /Fd"$(OUTDIR)\$(KONOHA).pdb"\
       /D_WINDOWS /DLIBICONV_PLUG\
       /I"..\include" /I".\include"

LINK32DLL_FLAGS=\
 /nologo /dll /LTCG /incremental:no /DEFAULTLIB:"ws2_32.lib" \
 /pdb:"$(OUTDIR)\$(KONOHA).pdb" /machine:I386 /out:"$(OUTDIR)\$(KONOHA).dll"\
 /implib:"$(OUTDIR)\$(KONOHA).lib"

LINK32_FLAGS=konoha.lib\
 /nologo /LTCG /subsystem:console /incremental:no /DEBUG\
 /out:"$(OUTDIR)\$(KONOHA).exe"

LINK32DLL_OBJS = \
	$(OUTDIR)\mt19937-64.obj \
	$(OUTDIR)\mt19937ar.obj \
	$(OUTDIR)\qsort.obj \
	$(OUTDIR)\strerror.obj \
	$(OUTDIR)\strlen.obj \
	$(OUTDIR)\konoha.obj \
	$(OUTDIR)\asm.obj \
	$(OUTDIR)\optimizer.obj \
	$(OUTDIR)\script.obj \
	$(OUTDIR)\term.obj \
	$(OUTDIR)\typing.obj \
	$(OUTDIR)\vmcodelibs.obj \
	$(OUTDIR)\vminline.obj \
	$(OUTDIR)\struct.obj \
	$(OUTDIR)\array.obj \
	$(OUTDIR)\bytes.obj \
	$(OUTDIR)\class.obj \
	$(OUTDIR)\closure.obj \
	$(OUTDIR)\context.obj \
	$(OUTDIR)\empty.obj \
	$(OUTDIR)\evidence.obj \
	$(OUTDIR)\exports.obj \
	$(OUTDIR)\iterator.obj \
	$(OUTDIR)\konohaapi.obj \
	$(OUTDIR)\map.obj \
	$(OUTDIR)\memory.obj \
	$(OUTDIR)\method.obj \
	$(OUTDIR)\number.obj \
	$(OUTDIR)\object.obj \
	$(OUTDIR)\os.obj \
	$(OUTDIR)\query.obj \
	$(OUTDIR)\security.obj \
	$(OUTDIR)\semantics.obj \
	$(OUTDIR)\stack.obj \
	$(OUTDIR)\stream.obj \
	$(OUTDIR)\string.obj \
	$(OUTDIR)\system.obj \
	$(OUTDIR)\thread.obj \
	$(OUTDIR)\time.obj \
	$(OUTDIR)\translator.obj \
	$(OUTDIR)\tuple.obj \
	$(OUTDIR)\updater.obj \

$(OUTDIR)\mt19937-64.obj : "../src/ext/mt19937-64.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\mt19937-64.obj" "../src/ext/mt19937-64.c"

$(OUTDIR)\mt19937ar.obj : "../src/ext/mt19937ar.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\mt19937ar.obj" "../src/ext/mt19937ar.c"

$(OUTDIR)\qsort.obj : "../src/ext/qsort.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\qsort.obj" "../src/ext/qsort.c"

$(OUTDIR)\strerror.obj : "../src/ext/strerror.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\strerror.obj" "../src/ext/strerror.c"

$(OUTDIR)\strlen.obj : "../src/ext/strlen.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\strlen.obj" "../src/ext/strlen.c"

$(OUTDIR)\konoha.obj : "../src/konoha.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\konoha.obj" "../src/konoha.c"

$(OUTDIR)\asm.obj : "../src/lang/asm.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\asm.obj" "../src/lang/asm.c"

$(OUTDIR)\optimizer.obj : "../src/lang/optimizer.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\optimizer.obj" "../src/lang/optimizer.c"

$(OUTDIR)\script.obj : "../src/lang/script.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\script.obj" "../src/lang/script.c"

$(OUTDIR)\term.obj : "../src/lang/term.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\term.obj" "../src/lang/term.c"

$(OUTDIR)\typing.obj : "../src/lang/typing.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\typing.obj" "../src/lang/typing.c"

$(OUTDIR)\vmcodelibs.obj : "../src/lang/vmcodelibs.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\vmcodelibs.obj" "../src/lang/vmcodelibs.c"

$(OUTDIR)\vminline.obj : "../src/lang/vminline.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\vminline.obj" "../src/lang/vminline.c"

$(OUTDIR)\struct.obj : "../src/main/apidata/struct.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\struct.obj" "../src/main/apidata/struct.c"

$(OUTDIR)\array.obj : "../src/main/array.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\array.obj" "../src/main/array.c"

$(OUTDIR)\bytes.obj : "../src/main/bytes.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\bytes.obj" "../src/main/bytes.c"

$(OUTDIR)\class.obj : "../src/main/class.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\class.obj" "../src/main/class.c"

$(OUTDIR)\closure.obj : "../src/main/closure.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\closure.obj" "../src/main/closure.c"

$(OUTDIR)\context.obj : "../src/main/context.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\context.obj" "../src/main/context.c"

$(OUTDIR)\empty.obj : "../src/main/empty.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\empty.obj" "../src/main/empty.c"

$(OUTDIR)\evidence.obj : "../src/main/evidence.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\evidence.obj" "../src/main/evidence.c"

$(OUTDIR)\exports.obj : "../src/main/exports.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\exports.obj" "../src/main/exports.c"

$(OUTDIR)\iterator.obj : "../src/main/iterator.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\iterator.obj" "../src/main/iterator.c"

$(OUTDIR)\konohaapi.obj : "../src/main/konohaapi.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\konohaapi.obj" "../src/main/konohaapi.c"

$(OUTDIR)\map.obj : "../src/main/map.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\map.obj" "../src/main/map.c"

$(OUTDIR)\memory.obj : "../src/main/memory.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\memory.obj" "../src/main/memory.c"

$(OUTDIR)\method.obj : "../src/main/method.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\method.obj" "../src/main/method.c"

$(OUTDIR)\number.obj : "../src/main/number.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\number.obj" "../src/main/number.c"

$(OUTDIR)\object.obj : "../src/main/object.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\object.obj" "../src/main/object.c"

$(OUTDIR)\os.obj : "../src/main/os.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\os.obj" "../src/main/os.c"

$(OUTDIR)\query.obj : "../src/main/query.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\query.obj" "../src/main/query.c"

$(OUTDIR)\security.obj : "../src/main/security.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\security.obj" "../src/main/security.c"

$(OUTDIR)\semantics.obj : "../src/main/semantics.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\semantics.obj" "../src/main/semantics.c"

$(OUTDIR)\stack.obj : "../src/main/stack.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\stack.obj" "../src/main/stack.c"

$(OUTDIR)\stream.obj : "../src/main/stream.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\stream.obj" "../src/main/stream.c"

$(OUTDIR)\string.obj : "../src/main/string.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\string.obj" "../src/main/string.c"

$(OUTDIR)\system.obj : "../src/main/system.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\system.obj" "../src/main/system.c"

$(OUTDIR)\thread.obj : "../src/main/thread.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\thread.obj" "../src/main/thread.c"

$(OUTDIR)\time.obj : "../src/main/time.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\time.obj" "../src/main/time.c"

$(OUTDIR)\translator.obj : "../src/main/translator.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\translator.obj" "../src/main/translator.c"

$(OUTDIR)\tuple.obj : "../src/main/tuple.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\tuple.obj" "../src/main/tuple.c"

$(OUTDIR)\updater.obj : "../src/main/updater.c"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\updater.obj" "../src/main/updater.c"

$(OUTDIR)\$(KONOHA).dll : $(OUTDIR) $(LINK32DLL_OBJS)
	$(LINK32) $(LINK32DLL_FLAGS) $(LINK32DLL_OBJS)
$(OUTDIR)\$(KONOHA).exe : $(OUTDIR) $(OUTDIR)\$(KONOHA).dll "$(OUTDIR)\konoha.obj"
	$(LINK32) $(LINK32_FLAGS) "$(OUTDIR)\$(KONOHA).lib" "$(OUTDIR)\$(KONOHA).obj"

