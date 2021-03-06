#!/usr/local/bin/konoha
/*
 * gen_tmake.k
 * masahiro@ynu 2009
 *
 * generator for T-shell makefile.
 *
 * how to use;
 * $ cd /path/to/konoha/
 * $ konoha ./gensrc/gen_tmake.k
 * and then, generate Makefile
 */


import "walk.k"

makefile = "./Makefile.common";

int main(String[] args)
{
    OutputStream o = new OutputStream(makefile,"w");
    o.println("""
# Automagically generated file.
# generated by gensrc/gen_tmake.k
# 
# Platform-independent settings for T-Shell environments

DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

VPATH = "./ $(shell find ../../ -type d) ./"

# -I../support/include must precede -I$(BD)/include, so as to avoid
# ../support/include/machine.h be loaded earlier
CFLAGS += -Wall -g3 -fmessage-length=0 -I../support/include -I$(BD)/include -I../../include -DKONOHA_ON_TB -DKONOHA_MONOLITHIC -DKNH_USING_BTRON

LDLIBS := -ldl $(LDLIBS)

program = konoha
library = lib$(program)

.PHONY: all
all: $(program) $(program).map
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
		o.println(%("libkonoha_la_SOURCES += ../%s{0}",p));
	}

    o.println("""
# added for T-Shell environment
libkonoha_la_SOURCES += ../support/src/getenv.c

konoha_SOURCES = ../src/konoha.c

objs = $(addsuffix .o,$(basename $(notdir $(libkonoha_la_SOURCES) $(konoha_SOURCES))))

$(library).a: $(objs)
	$(AR) $(ARFLAGS) $@ $^

$(program).out: $(library).a konoha.o
	$(LINK.o) -o $@ konoha.o -L. -l$(program) $(LDLIBS) 

$(program).map: $(program).out
	$(NM) $(NMFLAGS) $< > $@

$(program): $(program).out
	$(CP) $< $@
	$(STRIP) $@
	$(ETCBIN)/bzcomp $(BZCOMP_FLAGS) $@

.PHONY: dist
dist:
	$(RM) $(objs) konoha.o

.PHONY: clean
clean:
	$(RM) $(objs) $(program) $(program).out $(program).map $(library).a konoha.o Dependencies

ifdef DEPENDENCIES_OUTPUT
  $(DEPS): ; touch $(DEPS)
else
  $(DEPS): $(SRC) ; $(MAKEDEPS) $@ $?
endif
-include $(DEPS)

""");
    o.close();
    return 0;
}

