#!/usr/local/bin/konoha
import "walk.k";
/*
 * gen_automake.k
 * Copyright (c) 2010       Masahiro Ide <imasahiro9@gmail.com>
 *
 * how to use;
 * $ cd /path/to/konoha/
 * $ konoha ./gensrc/gen_nmake.k
 * and then, i generate "./arch/windows/Makefile";
 */

MAKEFILE = "./arch/windows/Makefile";

void main(String[] args)
{
	OutputStream o = new OutputStream(MAKEFILE,"w");
	o.print("""
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
""");
	files = [];
	foreach (String p in System.walk("./src")) {
		if(p.indexOf(".svn") >= 0) continue;
		if(p.endsWith(".c") == false ) continue;
		if(p.indexOf("main/apidata/") >= 0) {
			if(p.endsWith("struct.c") == false) continue;
		}
		path = p;
		f = p.split("/").pop().replace(".c", "");
		o.println(%("	$(OUTDIR)\\%s{0}.obj \\",f));
		files.add(path);
	}
	o.println("");
	foreach (String p in files) {
		String f = p.split("/").pop().replace(".c", "");
		o.println(%("""
$(OUTDIR)\%s{1}.obj : ".%s{0}"
	$(CC) $(CFLAGS) /Fo"$(OUTDIR)\%s{1}.obj" ".%s{0}"
""", p, f));
	}
	o.println("""
$(OUTDIR)\$(KONOHA).dll : $(OUTDIR) $(LINK32DLL_OBJS)
	$(LINK32) $(LINK32DLL_FLAGS) $(LINK32DLL_OBJS)
$(OUTDIR)\$(KONOHA).exe : $(OUTDIR) $(OUTDIR)\$(KONOHA).dll "$(OUTDIR)\konoha.obj"
	$(LINK32) $(LINK32_FLAGS) "$(OUTDIR)\$(KONOHA).lib" "$(OUTDIR)\$(KONOHA).obj"
""");

	o.close();
}
