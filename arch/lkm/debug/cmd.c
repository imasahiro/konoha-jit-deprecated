#include "debug.h"

void exec_fmt(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, char *fmt);
int op_null(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
	IDEBUG("%s","null");
	return 1;
}
int op_next(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
	IDEBUG("%s", "next");
	return 0;
}
int op_print(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
	//fprintf(stderr, "%s\n",cmd.buf);
	IDEBUG("%s","print");
	knh_bytes_t sp = B(" ");
	int loc = knh_bytes_indexOf(cmd, sp);
	if(loc == -1) {
		//fprintf(stderr, "???\n");
		return 1;
	}
	else {
		knh_bytes_t c = knh_bytes_last(cmd, loc + 1);
		if(c.len > 4) { // strlen("%s()")
			exec_fmt(ctx, sfp, out, (char*)c.buf);
		}
	}
	return 1;
}
int op_bt(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, knh_bytes_t cmd)
{
	IDEBUG("%s", "bt");
	// TODO
	// we have to imprement backtrace
	//knh_stack_writeStackTrace(ctx, sfp, out);
	//knh_write_EOL(ctx, out);
	return 1;
}

// copy from src/class/knh_write.c
// knh_vprintf_parseindex
char *parse_index(char *p, int *index)
{
	char *ptr = p+1;
	IDEBUG("%s",__func__);
	if(ptr && strlen(ptr) > 6) {// 6 == strlen(sfp[i])
		if(
				ptr[0] == '(' && 
				ptr[1] == 's' && 
				ptr[2] == 'f' && 
				//(ptr[3] == 'p' || ptr[3] == 'x') && 
				ptr[3] == 'p' && 
				ptr[4] == '[' && 
				isdigit(ptr[5]) && 
				ptr[6] == ']' && 
				ptr[7] == ')') {
			*index = ptr[5] - '0';
			ptr += 8;
			return ptr;
		}
	}
	return p;
}

#define VA_NOP      0
#define VA_INT      1
#define VA_FLOAT    2
#define VA_CHAR     3
#define VA_POINTER  4
#define VA_OBJECT   5
#define VA_TYPE     6


void exec_fmt(Ctx *ctx, knh_sfp_t *sfp, knh_OutputStream_t *out, char *fmt)
{
	char *c = fmt;
	int ch;
	int type = VA_NOP;
	int i = 0;

	IDEBUG("%s", __func__);
	while((ch = *c) != '\0') {
		c++;
		if(ch == '%') {
			ch = *c;
			if(ch == '%') {
				c++;
				continue;
			}
			c = parse_index(c++, &i);
			//fprintf(stderr, "ch=%c,c=%c,index=%d\n", ch, *c, index);
			//DBG2_P("bindex=%d, index=%d", bindex, index);
			switch(ch) {
				case 'i': case 'd':
					type = VA_INT;
					break;
				case 'f':
					type = VA_FLOAT;
					break;
				case 's':
					type = VA_CHAR;
					break;
				case 'p':
					type = VA_POINTER;
					break;
				case 'K': case 'k':
				case 'O': case 'o':
					type = VA_OBJECT;
					break;
				case 'T':
					type = VA_TYPE;
					break;
			}
		}
	}
	switch(type) {
		case VA_NOP: 
			break;
		case VA_INT: 
			knh_printf(ctx,out,"%s=%d\n",fmt,sfp[i].ivalue);
			break;
		case VA_FLOAT: 
			knh_printf(ctx,out,"%s="KNH_FLOAT_FMT"\n",fmt,sfp[i].fvalue);
			break;
		case VA_CHAR: 
			if(IS_String(sfp[i].s)) {
				knh_printf(ctx,out,"%s='%s'\n", fmt, sfp[i].s->str);
			} else {
				knh_printf(ctx, out, "Err. sfp[%d] is not String!", i);
			}
			break;
		case VA_POINTER: 
			knh_printf(ctx,out,"%s=%p\n", fmt, sfp[i].o);
			break;
		case VA_OBJECT: 
			knh_printf(ctx,out,"%s=", fmt);
			knh_format(ctx, out, METHODN__k, UP(sfp[i].o), KNH_NULL);
			knh_printf(ctx,out,"\n");
			break;
		case VA_TYPE: 
			knh_printf(ctx,out,"%s=%s\n", fmt, CLASSNo(sfp[i].o));
			break;
	}
}

