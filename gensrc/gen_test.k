#!/usr/local/bin/konoha
include "walk.k"
/*
 * gen_test.k
 * masahiro@ynu 2009
 *
 * how to use;
 * $ cd /path/to/konoha/
 * $ konoha ./gensrc/gen_test.k
 */

void OutputStream.writeHead() {
	this.print("""
/*
 * this Makefile is generated from konoha/gensrc/gen_automake.k
 * Copyright (C) 2009 Masahiro Ide.
 */
""");
}

String escape(String func) {
	f = func;
	f = f.replace(" ","_");
	f = f.replace("%","fmt_");
	f = f.replace("*","_");
	f = f.replace(",","_");
	f = f.replace(";","");
	f = f.replace("[]","_arr_");
	f = f.replace("(","_");
	f = f.replace(")","_");
	f = f.replace("!","_");
	f = f.replace("?","_");
	f = f.replace(":","_");
	f = f.replace("...","_it_");
	f = f.replace("..","_it_");
	f = f.replace(".","_");
	return f;
}

void OutputStream.konohaTest(String func) {
		this << %("""
test "%s{0}" {
%s{0}
}

""",func);
}

void OutputStream.cApiTest(String func) {
	s = escape(func);
	//print s;
	this << %("""
// %s{0}
TEST(%s{1}) {
	return true;
}

""",func,s);
}


void konoha_test(DictMap<String[]> funclist)
{
	OutputStream o = new OutputStream("test/Method.k.org","w");
	o.writeHead();
	foreach(String func in funclist["method"]) {
		o.konohaTest(func);
	}

	foreach(String func in funclist["mapper"]) {
		o.konohaTest(func);
	}

	o.close();
}

void cmethod_test(DictMap<String[]> funclist)
{
	OutputStream o = new OutputStream("test/Method.c.org","w");
	o.writeHead();
	o.print("""
#include "../include/konoha.h"
#define TEST(func) int func(Ctx *ctx, knh_sfp_t *sfp)
#ifndef true
#define true 1
#define false 0
#endif
""");
	Pair<String,String>[] f = [];
	foreach(String func in funclist["method"]) {
		o.cApiTest(func);
		f.add((func,escape(func)));
	}

	foreach(String func in funclist["mapper"]) {
		o.cApiTest(func);
		f.add((func,escape(func)));
	}

	o << """
typedef int  (*test_fmethod)(Ctx *, knh_sfp_t*);
struct ctest { char *name; test_fmethod func;};
""";
	o << "struct ctest ctest[] = {" << EOL;
	foreach(Pair p in f) {
		o << "{\"" + p.op0() + "\", " + p.op1() + "}," << EOL;
	}
	o << "};" << EOL;

	o.print("""
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int main()
{
	int i;
	int n = ARRAY_SIZE(ctest);
	konoha_t konoha = konoha_open(4096);
	Ctx *ctx = knh_beginContext(konoha.ctx);
	knh_sfp_t *sfp = KNH_LOCAL(ctx);
	for (i = 0; i < n; i++) {
		struct ctest *test = &ctest[i];
		if(test->func(ctx, sfp + 1) == true) {
			fprintf(stderr, "[PASS %3d/%3d] %s\\n",i,n,test->name);
		} else {
			fprintf(stderr, "[FAILED %3d/%3d] %s\\n",i,n,test->name);
			exit(1);
		}
	}
	KNH_LOCALBACK(ctx, sfp);
	knh_endContext(ctx);
	konoha_close(konoha);
	return 0;
}
""");
	o.close();
}

void main(String[] args)
{
	
	String[] filelist = [];
	DictMap<String[]> funclist = new DictMap<String[]>();
	funclist["mapper"] = [];
	funclist["method"] = [];

	foreach(String p in System.walk("./src")) {
		if(p.indexOf(".svn") >= 0) continue;
		if(p.endsWith(".c") == false ) continue;
		filelist << p;
	}

	foreach(String file in filelist) {
		ins = new InputStream(file, "r");
		int i = 0;
		foreach(String line in ins) {
			i++;
			if (line.startsWith("//##")) {
				func = "";
				if(line.opHas("method")) {
					func = line.split(" method ",1).pop();
					func = func.replace(";","");
					funclist["method"].add(func);
				} else if(line.opHas("mapper")) {
					func = line.split("mapper",1).pop();
					func = func.replace(";","");
					funclist["mapper"].add(func);
				}

				if(func != "") {
					print %("%s{0}:%d{1} %s{2}",file,i,func);
				}
			}
		}
		ins.close();
	}
	cmethod_test(funclist);
	konoha_test(funclist);
}
