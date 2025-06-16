#ifndef tok_h
#define tok_h

#include "common.h"
#include "str.h"

typedef enum {

	// tk_Start = 0,// Start tokens
	tk_EOF = 0,		// EOF
	tk_Ident,	// <ident>
	tk_Lit,		// literal
	tk_Type,	// idk

	tk_fn,
	tk_ret,

	tk_int32,
	tk_nil,

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

	tk_End,		// end tokens; here to calc TKCOUNT
} Tokentyp;

#define TKCOUNT (tk_End)

typedef struct {
	char* source;
	char* current;
	char* line;

	usize src_len;

	u64 pos, ln_no, ln_index;
} Scanner;

struct Token {
	Tokentyp typ;
	char* line;

	char* data;
	i64 val;

	usize pos, ln_no, ln_index;
};

typedef struct {
	struct Token* toks;
	i32 capacity;
	i32 length;
} vectk;

vectk Tokenizer(char* source, usize len);
void printTokens(vectk* vec);

#endif