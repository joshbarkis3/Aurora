#ifndef tok_h
#define tok_h

#include "common.h"
#include "str.h"

typedef enum {
	tk_Error = 0, 	// Error
	tk_EOF,			// EOF
	tk_Ident,		// ident
	tk_Type,		// idk
	
	tk_Lit,			// literals, you can figure it out
	tk_intLit,
	tk_fltLit,
	tk_chrLit,
	tk_strLit,

	tk_fn,			// keywords
	tk_ret,
	tk_break,
	tk_cont,
	tk_do,
	tk_while,
	tk_for,
	tk_if,
	tk_else,
	tk_goto,
	tk_switch,
	tk_case,
	tk_default,
	tk_auto,
	tk_import,
	tk_use,
	tk_typedef,

	// datatype keywords
	tk_int8,
	tk_int16,
	tk_int32,
	tk_int64,
	tk_uint1, // bool
	tk_uint8,
	tk_uint16,
	tk_uint32,
	tk_uint64,
	tk_float32,
	tk_float64,
	tk_float128,
	tk_nil,
	tk_str,
	tk_ptr,

	// symbols
	tk_lparen,		
	tk_rparen,
	tk_lbrace,
	tk_rbrace,
	tk_lbracket,
	tk_rbracket,

	tk_comma,
	tk_dot,
	tk_arrow,
	tk_colon,
	tk_semi,
	tk_addr,
	tk_hash,
	tk_bslash,

	// operators
	tk_assign,
	tk_add,
	tk_sub,
	tk_unsub,
	tk_mul,
	tk_div,
	tk_mod,
	tk_pow,
	tk_inc,
	tk_dec,
	tk_addeq,
	tk_subeq,
	tk_muleq,
	tk_diveq,
	tk_modeq,

	tk_and,
	tk_or,
	tk_xor,
	tk_not,
	tk_eqeqs,
	tk_noteqs,
	tk_less,
	tk_lesseq,
	tk_great,
	tk_greateq,

	tk_bitand,
	tk_bitor,
	tk_bitxor,
	tk_bitnot,
	tk_shiftL,
	tk_shiftR,

	tk_End,			// end tokens; here to calc TKCOUNT
} Tokentyp;

#define TKCOUNT (tk_End)

typedef struct {
	char* source;
	char* current;
	char* line;

	usize src_len;

	u64 pos, ln_no, ln_index;

	bool isErr;
} Scanner;

struct Token {
	Tokentyp typ;
	char* line;

	char* data;
	u64 Ival;
	f128 Fval;

	usize pos, ln_no, ln_index;
};

typedef struct {
	struct Token* toks;
	i32 capacity;
	i32 length;
} vectk;

vectk Tokenizer(char* source, usize len);
void printTokens(vectk* vec);
void freeVec(vectk* vec);

#endif