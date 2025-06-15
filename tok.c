#include "tok.h"
#include "memory.h"
#include <stdlib.h>

const str tkVal[TKCOUNT] = {
	// [tk_Start] = 0,
	[tk_EOF] = 0,
	[tk_Ident] = 0,
	[tk_Type] = 0,

	[tk_fn] = "fn",
	[tk_ret] = "return",

	[tk_int32] = "int32",
	[tk_nil] = "nil",

	[tk_lparen] = "(",
	[tk_rparen] = ")",
	[tk_lbrace] = "{",
	[tk_rbrace] = "}",
	[tk_rbracket] = "[",
	[tk_lbracket] = "]",
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
	// [tk_Start] = "tk_Start",
	[tk_EOF] = "tk_EOF",
	[tk_Ident] = "tk_ident",
	[tk_Lit] = "tk_Lit",
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

void initScanner(Scanner* scanner, str source, usize len) {
	scanner->source = source;
	scanner->current = source;
	scanner->line = source;

	scanner->src_len = len;

	scanner->pos = 0;
	scanner->ln_no = 1;
	scanner->ln_index = 1;
}

void initVec(vectk* vec) {
	vec->toks = NULL;
	vec->capacity = 0;
	vec->length = 0;
}

void apndToken(vectk* vec, struct Token newTok) {
	if(vec->capacity < vec->length + 1) {
		i32 oldCapa = vec->capacity;
		vec->capacity = GROW_CAPACITY(oldCapa);
		vec->toks = GROW_ARR(struct Token, vec->toks, oldCapa, vec->capacity);
	}

	vec->toks[vec->length] = newTok;
	vec->length++;
}

void freeVec(vectk* vec) {
	free(vec);
	initVec(vec);
}

void advance(Scanner* scanner) {
	scanner->current++;
	scanner->pos++;
	scanner->ln_index++;
}

void printTokens(vectk* vec) {
	if (vec->length <= 0) {
		puts("Vector Arr is empty");
	}
	for(int i = 0; i < vec->length; i++) {
		printf("Token {.typ: %s; .dat:\"%s\"; .val:%zu pos:%zu:%zu:%zu}\n", 
			tkToString[vec->toks[i].typ], vec->toks[i].data, vec->toks[i].val, 
			vec->toks[i].pos, vec->toks[i].ln_no, vec->toks[i].ln_index);
	}
}

vectk Tokenizer(str source, usize len) {
	Scanner scanner;
	vectk tokens;
	initScanner(&scanner, source, len);
	initVec(&tokens);

	// apndToken(&tokens, (struct Token){.typ = tk_fn, .line = NULL, .data = tkVal[tk_fn], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_Ident, .line = NULL, .data = "main", .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_lparen, .line = NULL, .data = tkVal[tk_lparen], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_nil, .line = NULL, .data = tkVal[tk_nil], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_rparen, .line = NULL, .data = tkVal[tk_rparen], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_arrow, .line = NULL, .data = tkVal[tk_arrow], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_int32, .line = NULL, .data = tkVal[tk_int32], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_rbrace, .line = NULL, .data = tkVal[tk_rbrace], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_ret, .line = NULL, .data = tkVal[tk_ret], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_Lit, .line = NULL, .data = "", .val = 2, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_semi, .line = NULL, .data = tkVal[tk_semi], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
	// apndToken(&tokens, (struct Token){.typ = tk_lbrace, .line = NULL, .data = tkVal[tk_lbrace], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});

	while(*scanner.current != '\0') {
		switch(*scanner.current) {
		case ' ':
		case '\t':
			break;
		case '\n':
			scanner.ln_no++;
			scanner.ln_index = 0;
			break;
		case '(':
			apndToken(&tokens, (struct Token){.typ = tk_lparen, .line = NULL, .data = tkVal[tk_lparen], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ')':
			apndToken(&tokens, (struct Token){.typ = tk_rparen, .line = NULL, .data = tkVal[tk_rparen], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '{':
			apndToken(&tokens, (struct Token){.typ = tk_lbrace, .line = NULL, .data = tkVal[tk_lbrace], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '}':
			apndToken(&tokens, (struct Token){.typ = tk_rbrace, .line = NULL, .data = tkVal[tk_rbrace], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		}
		advance(&scanner);
	}

	printTokens(&tokens);

	return tokens;
}
