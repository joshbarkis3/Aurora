#ifndef tok_h
#define tok_h

#include "common.h"

typedef enum {

	tk_Start = 0,// Start tokens
	tk_EOF,		// EOF
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
	str source;
	str current;
	str line;

	u64 pos, ln_no, column;
} Scanner;

struct Token {
	Tokentyp typ;
};

#endif