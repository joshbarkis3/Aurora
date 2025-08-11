#ifndef parser_h
#define parser_h

#include "ast.h"

struct Parser {
	const vectk* toks;
	struct Token* curr;
	u32 tkCt;
	u32 pos;
};

typedef enum {
	Prc_low,
	Prc_assign,
	Prc_or,
	Prc_and,
	Prc_Eq,
	Prc_Comp,
	Prc_adds,
	Prc_mults,
	Prc_unary,
	Prc_Call,
	Prc_Top,
} Precedence;

// void parse(const vectk* tk);

#endif