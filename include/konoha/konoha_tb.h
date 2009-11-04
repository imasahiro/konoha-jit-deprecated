#ifndef KONOHA_TB_H
#define KONOHA_TB_H

#ifdef KONOHA_ON_TB
#include <basic.h>

#define PRIdPTR "d"
#define PRIuPTR "u"

char* ALT_getenv(const char* name);
#endif

#endif
