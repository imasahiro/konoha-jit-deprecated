// THIS FILE WAS AUTOMATICALLY GENERATED

#define _ASM_(prefix, f) prefix _ ## f
#define MAX_CODEASM (31)
typedef int (*fcodeasm)(CTX ctx, knh_Stmt_t *stmt, knh_type_t reqt, int sfpidx);
typedef struct CodeAsm {
	fcodeasm asm_[MAX_CODEASM];
} CodeWriter_t;
struct CodeAsm CODEASM_ = {{
	_ASM_(ASM_PREFIX, ALLOW_asm),
	_ASM_(ASM_PREFIX, DENY_asm),
	_ASM_(ASM_PREFIX, RETURN_asm),
	_ASM_(ASM_PREFIX, YIELD_asm),
	_ASM_(ASM_PREFIX, IF_asm),
	_ASM_(ASM_PREFIX, SWITCH_asm),
	_ASM_(ASM_PREFIX, CASE_asm),
	_ASM_(ASM_PREFIX, WHILE_asm),
	_ASM_(ASM_PREFIX, DO_asm),
	_ASM_(ASM_PREFIX, FOR_asm),
	_ASM_(ASM_PREFIX, THROW_asm),
	_ASM_(ASM_PREFIX, PRINT_asm),
	_ASM_(ASM_PREFIX, ASSURE_asm),
	_ASM_(ASM_PREFIX, ASSERT_asm),
	_ASM_(ASM_PREFIX, REGISTER_asm),
	_ASM_(ASM_PREFIX, DECL_asm),
	_ASM_(ASM_PREFIX, LET_asm),
	_ASM_(ASM_PREFIX, TCAST_asm),
	_ASM_(ASM_PREFIX, OPR_asm),
	_ASM_(ASM_PREFIX, CALL_asm),
	_ASM_(ASM_PREFIX, NEW_asm),
	_ASM_(ASM_PREFIX, FUNCCALL_asm),
	_ASM_(ASM_PREFIX, ACALL_asm),
	_ASM_(ASM_PREFIX, ALT_asm),
	_ASM_(ASM_PREFIX, TRI_asm),
	_ASM_(ASM_PREFIX, AND_asm),
	_ASM_(ASM_PREFIX, OR_asm),
	_ASM_(ASM_PREFIX, W1_asm),
	_ASM_(ASM_PREFIX, SEND_asm),
	_ASM_(ASM_PREFIX, FMTCALL_asm),
	_ASM_(ASM_PREFIX, CALL1_asm),
}};
