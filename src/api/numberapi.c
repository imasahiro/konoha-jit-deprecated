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

#ifdef KNH_CC_METHODAPI

/* ------------------------------------------------------------------------ */
//## @Static method void System.setRandomSeed(Int seed);

static METHOD System_setRandomSeed(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_uint_t seed = IS_NULL(sfp[1].o) ? 0 : p_uinteger(sfp[1]);
	knh_srand(seed);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## @Static method Int! Int.random(Int? n);

static METHOD Int_random(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	knh_int_t n = knh_rand();
	if(IS_NOTNULL(sfp[1].o)) {
		knh_int_t max = p_integer(sfp[1]);
		if(max > 0) {
			n = n % max;
		}
	}
	KNH_RETURN_Int(ctx, sfp, n);
}

/* ------------------------------------------------------------------------ */
//## @Static method Float! Float.random();

static METHOD Float_random(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_RETURN_Float(ctx, sfp, knh_float_rand());
}

/* ------------------------------------------------------------------------ */
//## @Static method Int! Float.floatToIntBits(Float! n);

static METHOD Float_floatToIntBits(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_RETURN_Int(ctx, sfp, sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Static method Float! Float.intToFloatBits(Int! n);

static METHOD Float_intToFloatBits(Ctx *ctx, knh_sfp_t *sfp METHODARG)
{
	KNH_RETURN_Float(ctx, sfp, sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */

#endif /*KNH_CC_METHODAPI*/


#ifdef __cplusplus
}
#endif
