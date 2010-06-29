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

/* ************************************************************************ */

#include"commons.h"

/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_DEFAULTAPI

/* ------------------------------------------------------------------------ */
//## @Static method void System.setRandomSeed(Int seed);

static METHOD System_setRandomSeed(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_uint_t seed = Int_to(knh_uint_t, sfp[1]);
	knh_srand(seed);
	RETURNvoid_();
}

/* ------------------------------------------------------------------------ */
//## @Static method Int Int.random(Int n);

static METHOD Int_random(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	knh_uint_t n = knh_rand();
	knh_uint_t max = Int_to(knh_uint_t, sfp[1]);
	if(max > 0) {
		n = n % max;
	}
	RETURNi_(n);
}

/* ------------------------------------------------------------------------ */
//## @Static method Float! Float.random();

static METHOD Float_random(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNf_(knh_float_rand());
}

/* ------------------------------------------------------------------------ */
//## @Static method Int Float.floatToIntBits(Float! n);

static METHOD Float_floatToIntBits(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNi_(sfp[1].fvalue);
}

/* ------------------------------------------------------------------------ */
//## @Static method Float! Float.intToFloatBits(Int n);

static METHOD Float_intToFloatBits(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	RETURNf_(sfp[1].ivalue);
}

/* ------------------------------------------------------------------------ */

#endif /*K_USING_DEFAULTAPI*/


#ifdef __cplusplus
}
#endif
