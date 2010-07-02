/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2006-2010, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Team konohaken@googlegroups.com
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with K_UNDER_LGPL)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef KONOHA_H_
#define KONOHA_H_
#ifndef K_INTERNAL
#define K_EXPORTS   1
#endif

#ifdef HAVE_CONFIG_H
#include"../config.h"
#endif

/* configuration and dependency */

#ifndef KONOHA_ON_LKM
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>
#include<string.h>
#include<setjmp.h>
#include<stdarg.h>
#endif

#include"konoha/konoha_config.h"
//#include"konoha/konoha_tb.h"
//#include"konoha/konoha_lkm.h"
#include"konoha/konoha_deps.h"

/* types, symbols */

#include"konoha/konoha_t.h"
#include"konoha/konoha_class.h"
#include"konoha/konoha_grammar_.h"

/* functionality */

#include"konoha/konoha_glue.h"
#include"konoha/konoha_gc.h"
#include"konoha/konoha_vm.h"
#include"konoha/konoha_debug.h"

/* konohaapi, prototypes */

#include"konoha/konoha_api.h"
#include"konoha/inlinelibs.h"



#endif /*KONOHA_H_*/

