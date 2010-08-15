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
 * (1) GNU General Public License 2.0      (with    KONOHA_UNDER_GPL2)
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

#include"commons.h"


#ifdef __cplusplus 
extern "C" {
#endif

#ifdef K_USING_VMINLINE

//static int knh_stack_pushesp(Ctx *ctx, knh_sfp_t *sfp) /* thisidx */
//{
//	int i = -1;
//	knh_sfp_t *esp = ctx->esp + 3;
//	int delta = esp - sfp;
//	DBG_ASSERT(sfp < esp);
//	while(sfp + i < esp) {
//		klr_mov(ctx, esp[i].o, sfp[i].o);
//		esp[i].data = sfp[i].data;
//		i++;
//	}
//	klr_setesp(ctx, esp + delta);
//	return delta;
//}

static knh_bool_t knh_Exception_isa(Ctx *ctx, knh_Exception_t *e, knh_String_t *event)
{
	if(knh_bytes_strcasecmp(S_tobytes(event), S_tobytes(DP(e)->event)) != 0) {
		knh_ebi_t eid = knh_geteid(ctx, S_tobytes(event), EBI_unknown);
		if(eid != EBI_unknown) {
			return knh_expt_isa(ctx, DP(e)->eid, eid);
		}
		return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------ */

#endif/*K_USING_VMINLINE*/

#ifdef __cplusplus
}
#endif
