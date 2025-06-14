#include "tok.h"

const str tkVal[TKCOUNT] = {
	[tk_Start] = 0,
	[tk_EOF] = 0,
	[tk_Ident] = 0,
	[tk_Type] = 0,

	[tk_fn] = "fn",
	[tk_ret] = "return",

	[tk_int32] = "int32",
	[tk_nil] = "nil",

	[tk_lparen] = "(",
	[tk_rparen] = ")",
	[tk_lbrace] = "[",
	[tk_rbrace] = "]",
	[tk_rbracket] = "{",
	[tk_lbracket] = "}",
	[tk_comma] = ",",
	[tk_dot] = ".",
	[tk_arrow] = "->",
	[tk_colon] = ":",
	[tk_semi] = ";",
	[tk_addr] = "@",
};

// cool hack to get around using a ToString fn
// and a large switch case
const str tkToString[TKCOUNT] = {
	[tk_Start] = "tk_Start",
	[tk_EOF] = "tk_EOF",
	[tk_Ident] = "tk_ident",
	[tk_Type] = "tk_Type",

	[tk_fn] = "tk_fn",
	[tk_ret] = "tk_ret",

	[tk_int32] = "tk_int32",
	[tk_nil] = "tk_nil",

	[tk_lparen] = "tk_lparen",
	[tk_rparen] = "tk_rparen",
	[tk_lbrace] = "tk_lbrace",
	[tk_rbrace] = "tk_rbrace",
	[tk_lbracket] = "tk_lbracket",
	[tk_rbracket] = "tk_rbracket",
	[tk_comma] = "tk_comma",
	[tk_dot] = "tk_dot",
	[tk_arrow] = "tk_arrow",
	[tk_colon] = "tk_colon",
	[tk_semi] = "tk_semi",
	[tk_addr] = "tk_addr",
};

