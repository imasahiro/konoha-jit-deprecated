#!/usr/local/bin/konoha
/*
 * gen_automake.k
 * Copyright (c) 2010       Masahiro Ide <imasahiro9@gmail.com>
 *
 * how to use;
 * $ cd /path/to/konoha/
 * $ konoha ./gensrc/gen_automake.k
 * and then, i generate ./build/Makefile.am
 */

MAKEFILE = "./build/Makefile.am";
void Array.addAll(String[] list){
    foreach(Any l in list){
        this.add(l);
    }
}
String[] System.walk(String path) {
    String[] p = [];
    if(System.hasDir(path)){
        foreach(String t in System.listDir(path)){
            String newpath = path + "/" + t;
            if(System.hasDir(newpath)){
                p.addAll(System.walk(newpath));
            } else {
                p.add(newpath);
            }
        }
    }
    return p;
}

void main(String[] args) {
	OutputStream o = new OutputStream(MAKEFILE,"w");
	o.print("""
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
""");

	foreach (String p in System.walk("./src")) {
		if(p.indexOf(".svn") >= 0) continue;
		if(p.endsWith(".c") == false ) continue;
		if(p.endsWith("konoha.c")) continue;
		if(p.indexOf("main/apidata/") >= 0) {
			if(p.endsWith("struct.c") == false) continue;
		}

		OUT << p << EOL;
		o.println("libkonoha_la_SOURCES += ." + p);
	}

	o.println("libkonoha_la_includedir = $(includedir)");
	o.println("libkonoha_la_include_HEADERS = ../include/konoha.h");

	o.println("libkonoha_la_includekonohadir = $(includedir)/../include/konoha");
	o.println("libkonoha_la_includekonoha_HEADERS =");

	foreach (String p in System.walk("./include/konoha")) {
		if(p.indexOf(".svn") >= 0) continue;
		if(p.endsWith(".h") == false ) continue;
		if(p.endsWith("konoha.h")) continue;

		o.println("libkonoha_la_includekonoha_HEADERS += ." + p);
	}

	o.println("sampledir = $(prefix)/konoha/sample");
	o.println("dist_sample_SCRIPTS =");

	foreach (String p in System.walk("./dist/sample")) {
		if(p.indexOf(".svn") >= 0) continue;
		if(p.endsWith(".k") == false ) continue;
		if(p.endsWith("konoha.h")) continue;

		o.println("dist_sample_SCRIPTS += ." +p);
	}
	o.println("""
scriptklipdir = $(prefix)/konoha/script/klip
dist_scriptklip_SCRIPTS = ../dist/bin/klip.k""");

	o.println("""
klipdir = $(bindir)
dist_klip_SCRIPTS = ../dist/bin/klip

man_MANS = ../konoha.1

bin_PROGRAMS = konoha
konoha_SOURCES = ../src/konoha.c
konoha_INCLUDES = ../include
konoha_LDADD = libkonoha.la
""");

	o.close();
}

