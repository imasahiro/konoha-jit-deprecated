#ifndef LKM_H
#define LKM_H

struct func_data {
	char *cname;
	char *fname;
	knh_fmethod func;
};

void init_mtd(Ctx *ctx, struct func_data *data);
void Ctx_evalScript(Ctx *ctx, char *data);

#endif /* LKM_H */

