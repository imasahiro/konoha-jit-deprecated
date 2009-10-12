/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
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

/* ************************************************************************ */

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [structs] */


/* ======================================================================== */
/* [constructors] */

knh_KLRCode_t* new_KLRCode(Ctx *ctx, knh_uri_t uri, knh_bytes_t elf, knh_bytes_t dwarf)
{
	knh_KLRCode_t *o = (knh_KLRCode_t*)new_Object_bcid(ctx, CLASS_KLRCode, 0);

	DP(o)->uri = uri;
	DP(o)->code = (knh_code_t*)KNH_MALLOC(ctx, KNH_SIZE(elf.len));
	DP(o)->size = elf.len;
	knh_memcpy(DP(o)->code, elf.buf, elf.len);

	KNH_ASSERT(dwarf.len % sizeof(knh_dwarf_t) == 0);
	DP(o)->dsize = dwarf.len / sizeof(knh_dwarf_t);
	DP(o)->dwarf = (knh_dwarf_t*)KNH_MALLOC(ctx, DP(o)->dsize * sizeof(knh_dwarf_t));
	knh_memcpy(DP(o)->dwarf, dwarf.buf, dwarf.len);
	return o;
}

/* ======================================================================== */
/* [methods] */

METHOD knh_KLRCode_exec(Ctx *ctx, knh_sfp_t *sfp);

/* ------------------------------------------------------------------------ */

void knh_Method_setKLRCode(Ctx *ctx, knh_Method_t *mtd, knh_KLRCode_t *code)
{
	KNH_ASSERT(IS_KLRCode(code));
	if(knh_Method_isObjectCode(mtd)) {
		KNH_SETv(ctx, DP(mtd)->code, code);
	}else {
		KNH_INITv(DP(mtd)->code, code);
		knh_Method_setObjectCode(mtd, 1);
	}
	knh_Method_syncFunc(mtd, knh_KLRCode_exec);
	mtd->pc_start = DP(code)->code;
}


/* ------------------------------------------------------------------------ */

knh_code_t* knh_Method_pcstartNULL(knh_Method_t *mtd)
{
	if(knh_Method_isObjectCode(mtd)) {
		knh_KLRCode_t *o = (knh_KLRCode_t*)DP(mtd)->code;
		if(IS_KLRCode(o)) {
			return DP(o)->code;
		}
	}
	return NULL;
}

/* ------------------------------------------------------------------------ */

knh_bytes_t knh_KLRCode_tobytes(knh_KLRCode_t *o)
{
	KNH_ASSERT(IS_KLRCode(o));
	return new_bytes__2((char*)DP(o)->code, DP(o)->size);
}

/* ------------------------------------------------------------------------ */

char *knh_Method_file(Ctx *ctx, knh_Method_t *mtd)
{
	if(!knh_Method_isObjectCode(mtd) || !IS_KLRCode((Object*)DP(mtd)->code)) {
		return "(naitive)";
	}
	knh_KLRCode_t *o = (knh_KLRCode_t*)DP(mtd)->code;
	return FILEN(DP(o)->uri);
}

/* ------------------------------------------------------------------------ */

int knh_Method_pctoline(knh_Method_t *mtd, knh_code_t *pc)
{
	if(knh_Method_isObjectCode(mtd) && IS_KLRCode((Object*)DP(mtd)->code)) {
		knh_KLRCode_t *o = (knh_KLRCode_t*)DP(mtd)->code;
		int offset = pc - DP(o)->code;
		if(0 <= offset && offset <= DP(o)->size) {
			int i;
			for(i = 0; i < (int)DP(o)->dsize - 1; i++) {
				if(DP(o)->dwarf[i].offset <= offset && offset < DP(o)->dwarf[i+1].offset) {
					return DP(o)->dwarf[i].line;
				}
			}
			return DP(o)->dwarf[DP(o)->dsize-1].line;
		}
	}
	return 0;
}

/* ------------------------------------------------------------------------ */

void knh_Gamma_loadCompiledMethod(Ctx *ctx, knh_Gamma_t *abr)
{

}

/* ------------------------------------------------------------------------ */

knh_fmethod knh_Gamma_getCompiledMethod(Ctx *ctx, knh_Gamma_t *abr, knh_bytes_t cname, knh_bytes_t mname)
{
	return NULL;
}


/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
