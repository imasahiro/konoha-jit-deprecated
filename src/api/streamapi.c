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

#ifdef KNH_CC_METHODAPI

/* ------------------------------------------------------------------------ */
//## method InputStream InputStream.new(String! urn, String? mode);

static METHOD InputStream_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_InputStream_t *o = (knh_InputStream_t*)sfp[0].o;
	KNH_RETURN(ctx, sfp, knh_InputStream_open(ctx, o, sfp[1].s, sfp[2].s));
}

/* ------------------------------------------------------------------------ */
//## method Int! InputStream.getChar();

static METHOD InputStream_getChar(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Int(ctx, sfp, knh_InputStream_getc(ctx, (knh_InputStream_t*)sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## method Int! InputStream.read(Bytes! buf, Int? offset, Int? length);

static METHOD InputStream_read(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
    knh_Bytes_t *ba = (knh_Bytes_t*)sfp[1].o;
    knh_bytes_t buf = knh_Bytes_tobytes(ba);
    size_t offset = 0;
    if(IS_Int(sfp[2].o)) {
        offset = (size_t)sfp[2].ivalue;
        if(offset > buf.len) {
            KNH_THROW_OUTOFINDEX(ctx, offset, buf.len);
        }
        buf = knh_bytes_last(buf, offset);
    }
    if(IS_Int(sfp[3].o)) {
        size_t len = (size_t)sfp[3].ivalue;
        knh_Bytes_ensureSize(ctx, ba, offset + len);
        buf.len = len;
    }
    KNH_RETURN_Int(ctx, sfp, knh_InputStream_read(ctx, (knh_InputStream_t*)sfp[0].o, (char*)buf.buf, buf.len));
}

/* ------------------------------------------------------------------------ */
//## method String! InputStream.getEncoding();

static METHOD InputStream_getEncoding(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN(ctx, sfp, DP((knh_InputStream_t*)sfp[0].o)->enc);
}

/* ------------------------------------------------------------------------ */
//## method void InputStream.setEncoding(String? enc);

static METHOD InputStream_setEncoding(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_InputStream_setEncoding(ctx, (knh_InputStream_t*)sfp[0].o, sfp[1].s);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method Boolean! InputStream.isClosed();

static METHOD InputStream_isClosed(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_InputStream_isClosed(ctx, (knh_InputStream_t*)sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## method void InputStream.close();

static METHOD InputStream_close(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_InputStream_close(ctx, (knh_InputStream_t*)sfp[0].o);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method String? InputStream.readLine();

static METHOD InputStream_readLine(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_String_t *s = knh_InputStream_readLine(ctx, (knh_InputStream_t*)sfp[0].o);
	KNH_RETURN(ctx, sfp, s);
}

/* ------------------------------------------------------------------------ */
//## method Any? InputStream.readData();

static METHOD InputStream_readData(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	Object *v = knh_InputStream_readData(ctx, (knh_InputStream_t*)sfp[0].o);
	KNH_RETURN(ctx, sfp, v);
}

/* ======================================================================== */
/* [iterators] */

static
ITRNEXT knh_InputStream_line_next(Ctx *ctx, knh_sfp_t *sfp, int n)
{
	knh_Iterator_t *it = sfp[0].it;
	knh_InputStream_t *in = (knh_InputStream_t*)DP(it)->source;
	int ch;
	knh_cwb_t cwbbuf;
	knh_cwb_t *cwb = knh_cwb_open(ctx, &cwbbuf);

	while((ch = knh_InputStream_getc(ctx, in)) != EOF) {
		if(ch == 13) continue;
		if(ch == 10) {
			KNH_ITRNEXT(ctx, sfp, n, new_String__cwbconv(ctx, cwb, DP(in)->bconv));
		}
		knh_Bytes_putc(ctx, cwb->ba, ch);
	}
	if(knh_cwb_size(cwb) != 0) {
		KNH_ITRNEXT(ctx, sfp, n, new_String__cwbconv(ctx, cwb, DP(in)->bconv));
	}
	KNH_ITREND(ctx, sfp, n);
}

/* ------------------------------------------------------------------------ */
//## @Final mapper InputStream String..;

static MAPPER knh_InputStream_String__(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_MAPPED(ctx, sfp, new_Iterator(ctx, CLASS_String, sfp[0].o, knh_InputStream_line_next));
}

/* ======================================================================== */
/* [OutputStream] */

//## method OutputStream OutputStream.new(String! urn, String? mode);

static METHOD OutputStream_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *o = (knh_OutputStream_t*)sfp[0].o;
	KNH_RETURN(ctx, sfp, knh_OutputStream_open(ctx, o, sfp[1].s, sfp[2].s));
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.putChar(Int! ch);

static METHOD OutputStream_putChar(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Bytes_t *ba = DP(sfp[0].w)->ba;
	KNH_ASSERT(IS_Bytes(ba));
	knh_Bytes_putc(ctx, ba, sfp[1].ivalue);
	if(!knh_OutputStream_isStoringBuffer(sfp[0].w) && ba->size > DP(sfp[0].w)->driver->bufsiz) {
		DP(sfp[0].w)->driver->fwrite(ctx, DP(sfp[0].w)->fd, (char*)(ba)->buf, (ba)->size);
		knh_Bytes_clear(ba, 0);
	}
	DP(sfp[0].w)->size++;
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.write(Bytes! buf, Int? offset, Int? length);

static METHOD OutputStream_write(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *o = (knh_OutputStream_t*)sfp[0].o;
	knh_bytes_t buf = knh_Bytes_tobytes((knh_Bytes_t*)sfp[1].o);
	size_t offset = IS_NULL(sfp[2].o) ? 0 : knh_array_index(ctx, p_int(sfp[2]), buf.len);
	size_t len = IS_NULL(sfp[3].o) ? (buf.len - offset) : p_int(sfp[3]);
	if(offset + len > buf.len) len = buf.len - offset;
	buf.buf = &(buf.buf[offset]);
	buf.len = len;
	knh_OutputStream_write(ctx, o, buf);
}

/* ------------------------------------------------------------------------ */
//## method Boolean! OutputStream.isClosed();

static METHOD OutputStream_isClosed(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN_Boolean(ctx, sfp, knh_OutputStream_isClosed((knh_OutputStream_t*)sfp[0].o));
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.print(Any? value, ...);
//## method void OutputStream.opAppend(Any? value, ...);

static METHOD OutputStream_print(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *out = (knh_OutputStream_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int ac = knh_stack_argc(ctx, v);
	size_t i;
	for(i = 0; i < ac; i++) {
		if(IS_bString(v[i].o)) {
			if(v[i].s == TS_EOL) {
				knh_write_EOL(ctx, out);
			}
			else if(v[i].s == TS_BEGIN) {
				DP(out)->indent++;
			}
			else if(v[i].s == TS_END) {
				DP(out)->indent--;
			}
			else {
				knh_OutputStream_print_(ctx, out, __tobytes(v[i].s), 0);
			}
		}
		else {
			knh_stack_w(ctx, v + ac, v+i, METHODN__s, out, KNH_NULL);
		}
	}
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.println(Any? value, ...);

static METHOD OutputStream_println(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_SETv(ctx, ctx->esp[0].o, TS_EOL);
	KNH_SETESP(ctx, (ctx->esp+1));
	OutputStream_print(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.writeData(Any? data, ...);

static METHOD OutputStream_writeData(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_t *out = (knh_OutputStream_t*)sfp[0].o;
	knh_sfp_t *v = sfp + 1;
	int i, ac = knh_stack_argc(ctx, v);
	knh_intptr_t indent = DP(out)->indent;
	for(i = 0; i < ac; i++) {
		DP(out)->indent = 0;
		knh_stack_w(ctx, v + ac, v+i, METHODN__data, out, KNH_NULL);
		knh_putc(ctx, out, ';');
		knh_write_EOL(ctx, out);
	}
	DP(out)->indent = indent;
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.flush();

static METHOD OutputStream_flush(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_flush(ctx, sfp[0].w);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.clearBuffer();

static METHOD OutputStream_clearBuffer(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_clear(ctx, sfp[0].w);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.close();

static METHOD OutputStream_close(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_close(ctx, sfp[0].w);
	KNH_RETURN_void(ctx, sfp);
}

/* ------------------------------------------------------------------------ */
//## method String! OutputStream.getEncoding();

static METHOD OutputStream_getEncoding(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	KNH_RETURN(ctx, sfp, DP((knh_OutputStream_t*)sfp[0].o)->enc);
}

/* ------------------------------------------------------------------------ */
//## method void OutputStream.setEncoding(String? enc);

static METHOD OutputStream_setEncoding(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_OutputStream_setEncoding(ctx, sfp[0].w, sfp[1].s);
	KNH_RETURN_void(ctx, sfp);
}

/* ======================================================================== */
/* [Socket] */

/* ------------------------------------------------------------------------ */
//## method Socket! Socket.new(String! host, Int! port);

METHOD Socket_new(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Socket_t *so = (knh_Socket_t*)sfp[0].o;
	knh_bytes_t urn = __tobytes(sfp[1].s);
	char *ip_or_host = NULL;
	if(knh_bytes_startsWith(urn, STEXT("socket:"))) {
		ip_or_host = (char*)(knh_bytes_last(urn, 7)).buf;
	}
	else if(knh_bytes_startsWith(urn, STEXT("ip:"))) {
		ip_or_host = (char*)(knh_bytes_last(urn, 3)).buf;
	}
	else {
		ip_or_host = (char*)urn.buf;
	}
	KNH_SETv(ctx, DP(so)->urn, sfp[1].o);
	DP(so)->port = (int)sfp[2].ivalue;
	DP(so)->sd = knh_socket_open(ctx, ip_or_host, DP(so)->port, knh_Context_isStrict(ctx));
	if(DP(so)->sd != -1) {
		KNH_SETv(ctx, DP(so)->in,  new_InputStream__io(ctx, DP(so)->urn, (knh_io_t)DP(so)->sd, knh_getSocketDriver()));
		KNH_SETv(ctx, DP(so)->out, new_OutputStream__io(ctx, DP(so)->urn, (knh_io_t)DP(so)->sd, knh_getSocketDriver()));
	}
	KNH_RETURN(ctx, sfp, sfp[0].o);
}

/* ------------------------------------------------------------------------ */
//## method InputStream! Socket.getInputStream();

METHOD Socket_getInputStream(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Socket_t *so = (knh_Socket_t*)sfp[0].o;
	KNH_RETURN(ctx, sfp, DP(so)->in);
}

/* ------------------------------------------------------------------------ */
//## method OutputStream! Socket.getOutputStream();

METHOD Socket_getOutputStream(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Socket_t *so = (knh_Socket_t*)sfp[0].o;
	KNH_RETURN(ctx, sfp, DP(so)->out);
}

/* ------------------------------------------------------------------------ */
//## method void Socket.close();

METHOD Socket_close(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Socket_t *so = (knh_Socket_t*)sfp[0].o;
	if(DP(so)->sd != -1) {
		knh_InputStream_close(ctx, DP(so)->in);
		knh_OutputStream_close(ctx, DP(so)->out);
		knh_socket_close(ctx, DP(so)->sd);
		DP(so)->sd = -1;
	}
}

/* ------------------------------------------------------------------------ */
//## method Boolean! Socket.isClosed();

METHOD Socket_isClosed(Ctx *ctx, knh_sfp_t *sfp METHODOPT)
{
	knh_Socket_t *so = (knh_Socket_t*)sfp[0].o;
	KNH_RETURN_Boolean(ctx, sfp, (DP(so)->sd == -1));
}

/* ------------------------------------------------------------------------ */

#endif /*KNH_CC_METHODAPI*/


#ifdef __cplusplus
}
#endif
