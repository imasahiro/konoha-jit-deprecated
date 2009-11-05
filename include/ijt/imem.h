#ifndef IJT_MEM_H
#define IJT_MEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

// if you want to disassemble imem's data,
// please enable "IMEM_DISASSEMBLE".
// output data's filename is "disasm.o"
#define IMEM_DISASSEMBLE 

// if define it, using inline function.
#define IMEM_USING_INLINE

typedef unsigned char uchar_t;
typedef unsigned short ushort_t;

typedef struct mem_label {
    uchar_t opcode;
    uchar_t size;
    ushort_t pos;
    short addr;
} lstack_t;

typedef struct mem_struct {
    uchar_t* addr;
    ushort_t size;
    ushort_t index;

    lstack_t* lhead;
    lstack_t* lstack;
} imem_t;

#define IMEM_INIT_SIZE 1
#define LSTACK_INIT_SIZE 256

static void *checked_malloc(size_t size)
{
    void* p = malloc(size);
    assert(p);
    memset(p,0,size);
    return p;
}

static void checked_free(void *p, size_t size)
{
    assert(p);
    free(p);
    memset(p, 0, size);
}

static lstack_t *lstack_new (size_t size)
{
    size_t _size = (size < LSTACK_INIT_SIZE) ? size : LSTACK_INIT_SIZE + 1;
    lstack_t *stack = (lstack_t *) checked_malloc(sizeof(lstack_t) * _size);
    stack[0].size = _size;
    return &stack[1];
}

static void lstack_delete (lstack_t* lstack)
{
    lstack_t *stack = NULL;
    size_t size;
    if(!lstack)
        return;

    stack = &lstack[-1];
    size  = stack->size;
    checked_free(stack,sizeof(lstack_t) * size);
}


static imem_t *imem_new (size_t size)
{
    imem_t *mem = (imem_t *) checked_malloc(sizeof(imem_t));
    size_t pageSize = sysconf(_SC_PAGESIZE);
    if (size < IMEM_INIT_SIZE) {
        size = IMEM_INIT_SIZE;
    }
    size *= pageSize;
    //uchar_t *addr = (uchar_t *) checked_malloc(size);
    uchar_t *addr = (uchar_t *) valloc(size);

    mprotect(addr, size, PROT_READ | PROT_WRITE | PROT_EXEC);

    mem->index = 0;
    mem->size  = size;
    mem->addr  = addr;

    mem->lhead = lstack_new(1024);
    mem->lstack = mem->lhead;
    return mem;
}

static void imem_free(imem_t *mem)
{
    checked_free(mem->addr,mem->size);
    checked_free(mem, sizeof(imem_t));
}

static inline void imem_write(imem_t *mem, unsigned char *op,size_t len)
{
    size_t i;
    size_t idx = mem->index;
    for (i = 0; i < len; i++) {
        mem->addr[idx+i] = op[i];
    }
    mem->index += len;
}

#ifdef IMEM_USING_INLINE
static void imem_push(imem_t *mem, unsigned char x)
{
    //assert(mem->index < mem->size);
    mem->addr[mem->index++] = x;
}
#else 
#define imem_push(mem, x) \
{                                       \
    assert(mem->index < mem->size);     \
    mem->addr[mem->index++] = x;        \
} 
#endif

static void imem_disassemble(imem_t *mem)
{
#ifdef IMEM_DISASSEMBLE
	char *fname[] = {"out1.o","out2.o"};
	static int i = 0;
	FILE* fp = fopen(fname[i++],"w");
	fwrite(mem->addr,1,mem->size,fp);
	fclose(fp);
#endif
}

static void imem_complete(imem_t *mem)
{
    imem_disassemble(mem);
    //lstack_delete(mem->lhead);
}

static void imem_setLabel(imem_t *mem, int opcode, int opsize, int jmpaddr)
{
    lstack_t* stack = mem->lstack;
    stack->opcode = opcode;
    stack->pos  = mem->index;
    stack->size = opsize;
    stack->addr = jmpaddr;
    mem->lstack = &stack[1];
}

#endif /* end of include guard: IJT_MEM_H */
