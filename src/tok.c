#include "tok.h"
#include "common.h"
#include "memory.h"

#define IdentMaxSize ((usize) 255)

char* tkVal[TKCOUNT] = {
	[tk_Error] = 0,
	[tk_EOF] = "\\0",
	[tk_Ident] = 0,
	[tk_Type] = 0,
	
	[tk_Lit] = 0,
	[tk_intLit] = 0,
	[tk_fltLit] = 0,
	[tk_chrLit] = 0,
	[tk_strLit] = 0,

	[tk_fn] = "fn",
	[tk_ret] = "return",
	[tk_break] = "break",
	[tk_cont] = "continue",
	[tk_do] = "do",
	[tk_while] = "while",
	[tk_for] = "for",
	[tk_if] = "if",
	[tk_else] = "else",
	[tk_goto] = "goto",
	[tk_switch] = "switch",
	[tk_case] = "case",
	[tk_default] = "default",
	[tk_auto] = "auto",
	[tk_import] = "import",
	[tk_use] = "use",
	[tk_typedef] = "typedef",
	[tk_struct] = "struct",
	[tk_enum] = "enum",
	[tk_union] = "union",
	[tk_const] = "const",
	[tk_static] = "static",
	[tk_reg] = "register",
	[tk_volatile] = "volatile",

	[tk_int8] = "int8",
	[tk_int16] = "int16",
	[tk_int32] = "int32",
	[tk_int64] = "int64",
	[tk_uint1] = "uint1", 
	[tk_uint8] = "uint8",
	[tk_uint16] = "uint16",
	[tk_uint32] = "uint32",
	[tk_uint64] = "uint64",
	[tk_float32] = "float32",
	[tk_float64] = "float64",
	[tk_float128] = "float128",
	[tk_nil] = "nil",
	[tk_str] = "str",
	[tk_ptr] = "ptr",

	[tk_lparen] = "(",
	[tk_rparen] = ")",
	[tk_lbrace] = "{",
	[tk_rbrace] = "}",
	[tk_lbracket] = "[",
	[tk_rbracket] = "]",

	[tk_comma] = ",",
	[tk_dot] = ".",
	[tk_arrow] = "->",
	[tk_colon] = ":",
	[tk_semi] = ";",
	[tk_addr] = "@",
	[tk_hash] = "#",
	[tk_bslash] = "\\",

	[tk_assign] = "=",
	[tk_add] = "+",
	[tk_sub] = "-",
	[tk_unsub] = "-",
	[tk_mul] = "*",
	[tk_div] = "/",
	[tk_mod] = "%",
	[tk_pow] = "Not implemented",
	[tk_inc] = "++",
	[tk_dec] = "--",
	[tk_addeq] = "+=",
	[tk_subeq] = "-=",
	[tk_muleq] = "*=",
	[tk_diveq] = "/=",
	[tk_modeq] = "%=",

	[tk_and] = "&",
	[tk_or] = "|",
	[tk_xor] = "^",
	[tk_not] = "!",
	[tk_eqeqs] = "==",
	[tk_noteqs] = "!=",
	[tk_less] = "<",
	[tk_lesseq] = "<=",
	[tk_great] = ">",
	[tk_greateq] = ">=",

	[tk_bitand] = "&&",
	[tk_bitor] = "||",
	[tk_bitxor] = "^^",
	[tk_bitnot] = "!!",
	[tk_shiftL] = "<<",
	[tk_shiftR] = ">>",
};

// cool hack to get around using a ToString fn
// and a large switch case
const char* tkToString[TKCOUNT] = {
	[tk_Error] = "tk_Error",
	[tk_EOF] = "tk_EOF",
	[tk_Ident] = "tk_ident",
	[tk_Type] = "tk_Type",
	
	[tk_Lit] = "tk_Lit",
	[tk_intLit] = "tk_IntLit",
	[tk_fltLit] = "tk_fltLit",
	[tk_chrLit] = "tk_chrLit",
	[tk_strLit] = "tk_StrLit",

	[tk_fn] = "tk_fn",
	[tk_ret] = "tk_ret",
	[tk_break] = "tk_break",
	[tk_cont] = "tk_cont",
	[tk_do] = "tk_do",
	[tk_while] = "tk_while",
	[tk_for] = "tk_for",
	[tk_if] = "tk_if",
	[tk_else] = "tk_else",
	[tk_goto] = "tk_goto",
	[tk_switch] = "tk_switch",
	[tk_case] = "tk_case",
	[tk_default] = "tk_default",
	[tk_auto] = "tk_auto",
	[tk_import] = "tk_import",
	[tk_use] = "tk_use",
	[tk_typedef] = "tk_typedef",
	[tk_struct] = "tk_struct",
	[tk_enum] = "tk_enum",
	[tk_union] = "tk_union",
	[tk_const] = "tk_const",
	[tk_static] = "tk_static",
	[tk_reg] = "tk_reg",
	[tk_volatile] = "tk_volatile",

	[tk_int8] = "tk_int8",
	[tk_int16] = "tk_int16",
	[tk_int32] = "tk_int32",
	[tk_int64] = "tk_int64",
	[tk_uint1] = "tk_uint1", 
	[tk_uint8] = "tk_uint8",
	[tk_uint16] = "tk_uint16",
	[tk_uint32] = "tk_uint32",
	[tk_uint64] = "tk_uint64",
	[tk_float32] = "tk_float32",
	[tk_float64] = "tk_float64",
	[tk_float128] = "tk_float128",
	[tk_nil] = "tk_nil",
	[tk_str] = "tk_str",
	[tk_ptr] = "tk_ptr",

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
	[tk_hash] = "tk_hash",
	[tk_bslash] = "tk_bslash",

	[tk_assign] = "tk_assign",
	[tk_add] = "tk_add",
	[tk_sub] = "tk_sub",
	[tk_unsub] = "tk_unsub",
	[tk_mul] = "tk_mul",
	[tk_div] = "tk_div",
	[tk_mod] = "tk_mod",
	[tk_pow] = "Not implemented",
	[tk_inc] = "tk_inc",
	[tk_dec] = "tk_dec",
	[tk_addeq] = "tk_addeq",
	[tk_subeq] = "tk_subeq",
	[tk_muleq] = "tk_muleq",
	[tk_diveq] = "tk_diveq",
	[tk_modeq] = "tk_modeq",

	[tk_and] = "tk_and",
	[tk_or] = "tk_or",
	[tk_xor] = "tk_xor",
	[tk_not] = "tk_not",
	[tk_eqeqs] = "tk_eqeqs",
	[tk_noteqs] = "tk_noteqs",
	[tk_less] = "tk_less",
	[tk_lesseq] = "tk_lesseq",
	[tk_great] = "tk_great",
	[tk_greateq] = "tk_greateq",

	[tk_bitand] = "tk_bitand",
	[tk_bitor] = "tk_bitor",
	[tk_bitxor] = "tk_bitxor",
	[tk_bitnot] = "tk_bitnot",
	[tk_shiftL] = "tk_shiftL",
	[tk_shiftR] = "tk_shiftR",
};

static void initScanner(Scanner* scanner, char* source, usize len) {
	scanner->source = source;
	scanner->current = source;
	scanner->line = source;

	scanner->src_len = len;

	scanner->pos = 0;
	scanner->ln_no = 1;
	scanner->ln_index = 1;

	scanner->isErr = false;
}

static void initVec(vectk* vec) {
	vec->toks = NULL;
	vec->capacity = 0;
	vec->length = 0;
}

static void apndToken(vectk* vec, struct Token newTok) {
	if(vec->capacity < vec->length + 1) {
		i32 oldCapa = vec->capacity;
		vec->capacity = GROW_CAPACITY(oldCapa);
		vec->toks = GROW_ARR(struct Token, vec->toks, oldCapa, vec->capacity);
	}

	vec->toks[vec->length] = newTok;
	vec->length++;
}

void freeVec(vectk* vec) {
	for(i32 i = 0; i < vec->length; i++) {
		if(vec->toks[i].typ == tk_Ident || vec->toks[i].typ == tk_fltLit || vec->toks[i].typ == tk_intLit || vec->toks[i].typ == tk_strLit || vec->toks[i].typ == tk_Error) {
			// free(vec->toks[i].data);
		}
		free(vec->toks[i].line);
	}
	free(vec->toks);
}

static void advance(Scanner* scanner) {
	scanner->current++;
	scanner->pos++;
	scanner->ln_index++;
}

static bool isalph(char ch) {
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

static bool isNumb(char ch) {
	return ('0' <= ch && ch <= '9');
}

static char* getstrLine(Scanner* scanner, char* buf) {
	const char* end = strchr(scanner->line, '\n');
	const usize llen = end == NULL ? strlen(scanner->line) : ((usize) (end - scanner->line));

	if(buf == NULL) {
		buf = malloc(llen + 1);
	}

	memcpy(buf, scanner->line, llen);
	buf[llen] = '\0';

	return buf;
}

static void tokenizeKeywordIndent(Scanner* scanner, vectk* vec) {
	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
	char *strs = calloc(IdentMaxSize + 1, sizeof(char));
	char* errline = NULL;
	i32 i = 0;
	while(isalph(*scanner->current) || isNumb(*scanner->current) || *scanner->current == '_') {
		if(i == 255) {
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Indentifier/Keywords have a max size of 255 chars", pos, ln_no, column);
			free(scanner->source);
			exit(2);
		}

		strs[i] = *scanner->current;
		advance(scanner);
		i++;
	}

	#define STRCMP(s1, s2) (strcmp(s1, s2) == 0)

	if(STRCMP(strs, "fn")) {
		apndToken(vec, (struct Token) {.typ = tk_fn, .line = getstrLine(scanner, errline), .data = tkVal[tk_fn], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "return")) {
		apndToken(vec, (struct Token) {.typ = tk_ret, .line = getstrLine(scanner, errline), .data = tkVal[tk_ret], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "break")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_break, .line = getstrLine(scanner, errline), .data = tkVal[tk_break], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "continue")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_cont, .line = getstrLine(scanner, errline), .data = tkVal[tk_cont], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "do")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_do, .line = getstrLine(scanner, errline), .data = tkVal[tk_do], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "while")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_while, .line = getstrLine(scanner, errline), .data = tkVal[tk_while], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "for")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_for, .line = getstrLine(scanner, errline), .data = tkVal[tk_for], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "if")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_if, .line = getstrLine(scanner, errline), .data = tkVal[tk_if], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "else")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_else, .line = getstrLine(scanner, errline), .data = tkVal[tk_else], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "goto")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_goto, .line = getstrLine(scanner, errline), .data = tkVal[tk_goto], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "switch")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_switch, .line = getstrLine(scanner, errline), .data = tkVal[tk_switch], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "case")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_case, .line = getstrLine(scanner, errline), .data = tkVal[tk_case], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "default")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_default, .line = getstrLine(scanner, errline), .data = tkVal[tk_default], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "auto")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_auto, .line = getstrLine(scanner, errline), .data = tkVal[tk_auto], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "import")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_import, .line = getstrLine(scanner, errline), .data = tkVal[tk_import], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "use")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_use, .line = getstrLine(scanner, errline), .data = tkVal[tk_use], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "typedef")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_typedef, .line = getstrLine(scanner, errline), .data = tkVal[tk_typedef], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "struct")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_struct, .line = getstrLine(scanner, errline), .data = tkVal[tk_struct], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "enum")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_enum, .line = getstrLine(scanner, errline), .data = tkVal[tk_enum], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "union")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_union, .line = getstrLine(scanner, errline), .data = tkVal[tk_union], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "static")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_static, .line = getstrLine(scanner, errline), .data = tkVal[tk_static], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "register")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_reg, .line = getstrLine(scanner, errline), .data = tkVal[tk_reg], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "volatile")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_volatile, .line = getstrLine(scanner, errline), .data = tkVal[tk_volatile], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "i8") || STRCMP(strs, "int8") || STRCMP(strs, "sbyte")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_int8, .line = getstrLine(scanner, errline), .data = tkVal[tk_int8], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "i16") || STRCMP(strs, "int16") || STRCMP(strs, "short")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_int16, .line = getstrLine(scanner, errline), .data = tkVal[tk_int16], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "i32") || STRCMP(strs, "int32") || STRCMP(strs, "int")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_int32, .line = getstrLine(scanner, errline), .data = tkVal[tk_int32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "i64") || STRCMP(strs, "int64") || STRCMP(strs, "long") || STRCMP(strs, "isize")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_int64, .line = getstrLine(scanner, errline), .data = tkVal[tk_int64], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "u1") || STRCMP(strs, "uint1") || STRCMP(strs, "bool")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_uint1, .line = getstrLine(scanner, errline), .data = tkVal[tk_uint1], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "u8") || STRCMP(strs, "uint8") || STRCMP(strs, "byte") || STRCMP(strs, "char")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_uint8, .line = getstrLine(scanner, errline), .data = tkVal[tk_uint8], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "u16") || STRCMP(strs, "uint16") || STRCMP(strs, "ushort")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_uint16, .line = getstrLine(scanner, errline), .data = tkVal[tk_uint16], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "u32") || STRCMP(strs, "uint32") || STRCMP(strs, "uint")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_uint32, .line = getstrLine(scanner, errline), .data = tkVal[tk_uint32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "u64") || STRCMP(strs, "uint64") || STRCMP(strs, "ulong") || STRCMP(strs, "usize")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_uint64, .line = getstrLine(scanner, errline), .data = tkVal[tk_uint64], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "f32") || STRCMP(strs, "float32") || STRCMP(strs, "float")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_float32, .line = getstrLine(scanner, errline), .data = tkVal[tk_float32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "f64") || STRCMP(strs, "float64") || STRCMP(strs, "double")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_float64, .line = getstrLine(scanner, errline), .data = tkVal[tk_float64], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "f128") || STRCMP(strs, "float128") || STRCMP(strs, "ldouble")) { 
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_float128, .line = getstrLine(scanner, errline), .data = tkVal[tk_float128], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "nil") || STRCMP(strs, "u0") || STRCMP(strs, "uint0")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_nil, .line = getstrLine(scanner, errline), .data = tkVal[tk_nil], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "str")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_str, .line = getstrLine(scanner, errline), .data = tkVal[tk_str], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (STRCMP(strs, "ptr")) {
		free(strs);
		apndToken(vec, (struct Token) {.typ = tk_ptr, .line = getstrLine(scanner, errline), .data = tkVal[tk_ptr], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else {
		apndToken(vec, (struct Token){.typ=tk_Ident, .line = getstrLine(scanner, errline), .data = strs, .pos = pos, .ln_no = ln_no, .ln_index = column});
	}	
}
/*
helper function to tokenize identifiers or keywords.
Memory Leaks may be found here -- I did try to clean them up
*/

void handleInvalidIndent(Scanner* scanner, char *s, i32 i, vectk* vec, u64 pos, u64 ln_no, u64 column) {
	while(isalph(*scanner->current) || isNumb(*scanner->current)) {
		s[i] = *scanner->current;
		advance(scanner);
		i++;
	}
	char* errline = NULL;
	errline = getstrLine(scanner, errline);
	fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Invalid Identifier name: \"%s\"\n%8zu | %s\n%8c |%*c\n", pos, ln_no, column, s, ln_no, errline, ' ', (int)column + 1, '^');
	apndToken(vec, (struct Token){.typ=tk_Error, .line=errline, .data = s, .pos = pos, .ln_no = ln_no, .ln_index = column});
	scanner->isErr = true;
}

/*
maybe replace this with
	int start = pos;
	int seen_dot = 0;

	while (isdigit(peek()) || peek() == '.') {
	    if (peek() == '.') seen_dot++;
	    advance();
	}

	char buf[64];
	strncpy(buf, src + start, pos - start); // -> strncpy(char* buffer, u32 offset, u32 end)
	buf[pos - start] = '\0';

	if (seen_dot > 0) {
	    return (Token){.type = TOKEN_FLOAT, .fvalue = atof(buf)};
	} else {
	    return (Token){.type = TOKEN_INT, .ivalue = atoi(buf)};
	}

	needs modification to account for 
		multiple '.'s | 'E|e's,
		max size of buffer


Tokenizes Numbers, Floats and integers
Does not account for positive or negative. handled in parser with unary sub

*/

static void tokenizeNumbers(Scanner* scanner, vectk* vec) {
	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
	bool hasDot = false;
	bool isExp = false;
	bool isErr = false;
	char* errline = NULL;
	
	i32 i = 0;
	char* strs = calloc(IdentMaxSize + 1, sizeof(char));

	
	while(isNumb(*scanner->current) || *scanner->current == '.' || *scanner->current == 'e' || *scanner->current == 'E') {
		if(i == IdentMaxSize) {
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Numbers have a max size of 255 chars\n", pos, ln_no, column);
			exit(2);
		}

		if(*scanner->current == '.') {
			if(hasDot) {
				if(!isErr && hasDot) {
					errline = getstrLine(scanner, errline);
					fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Invalid Suffix for float \n%8zu | %s\n%8c |%*c\n", pos, ln_no, column, ln_no, errline, ' ', (int)scanner->ln_index + 1, '^');
				}
				scanner->isErr = true;
				isErr = true;
			} else {
				hasDot = true;	
			}
		} 
		if ((*scanner->current == 'e' || *scanner->current == 'E')) {
			if(isExp) {
				if(!isErr && isExp) {
					errline = getstrLine(scanner, errline);
					fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Floats cannot have more than one exp\n%8zu | %s\n%8c |%*c\n", pos, ln_no, column, ln_no, errline, ' ', (int)scanner->ln_index + 1, '^');
				}
				scanner->isErr = true;
				isErr = true;
			} else {
				isExp = true;	
			}

			if (*(scanner->current + 1) == '-') {
				strs[i] = *scanner->current;
				advance(scanner);
				i++;
			}
		}

		strs[i] = *scanner->current;
		advance(scanner);
		i++;

		if (isalph(*scanner->current) && !(*scanner->current == 'e' || *scanner->current == 'E')) {
			handleInvalidIndent(scanner, strs, i, vec, pos, ln_no, column);
			return;
		}
	}

	if ((hasDot || isExp) && !isErr) {
		apndToken(vec, (struct Token){.typ=tk_fltLit, .line = getstrLine(scanner, errline), .data=strs, .Fval = atof(strs), .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (!hasDot && !isExp && !isErr) {
		apndToken(vec, (struct Token){.typ=tk_intLit, .line = getstrLine(scanner, errline), .data=strs, .Ival = atoll(strs), .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else {
		errline = getstrLine(scanner, errline);
		apndToken(vec, (struct Token){.typ=tk_Error, .line = errline, .data=strs, .pos = pos, .ln_no = ln_no, .ln_index = column});
	}

	// free(s.string); memory leaker

	return;
}



// static void AnalyzeStr(Scanner* scanner, vectk* vec) {
// 	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
// 	str strLit;
// 	StrInit(&strLit, "", 8);
// 	// todo

// 	apndToken(vec, (struct Token){.typ = tk_chrLit, .line = NULL, .data = strLit.string, pos, ln_no, column});
// }

// static void AnalyzeChr(Scanner* scanner, vectk* vec) {
// 	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
// 	char chrLit[2] = "\0";
// 	// todo

// 	apndToken(vec, (struct Token){.typ = tk_chrLit, .line = NULL, .data = chrLit, .Ival = atoi(chrLit), pos, ln_no, column});
// 	return;
// }

// *scanner->current ==> '/'
// "" + 1 ==> '*'

static void ignoreBlockComment(Scanner* scanner) {
	// printf("current: %c %d \n", *scanner->current, *scanner->current);
	advance(scanner); // *scanner->current ==> '/' --> '*'
	// printf("current: %c %d\n", *scanner->current, *scanner->current);
	advance(scanner); // *scanner->current ==> '*' --> character after '*'
	// printf("current: %c %d\n", *scanner->current, *scanner->current);
	while(true) {
		advance(scanner);
		// printf("current: %c %d\n", *scanner->current, *scanner->current);
		if(*scanner->current == '\n') {
			scanner->ln_no++;
			scanner->ln_index = 0;
			scanner->line = scanner->current + 1;
		} else if(*scanner->current == '*' && *(scanner->current + 1) == '/') {
			// printf("next: %c %d\n", *(scanner->current + 1), *scanner->current);
			break;
		} else if(*scanner->current == '\0') {
			fprintf(stderr, "Tokenizer Error: Block comment unfinished, Hit EOF\n");
			exit(2);
		}
		
	}
	advance(scanner); // *scanner->current ==> '*' --> '/'
	// puts("stopping");
}

static void ignoreLineComment(Scanner* scanner) {
	while(*scanner->current != '\n') {
		advance(scanner);
	}
}

void printTokens(vectk* vec) {
	if (vec->length <= 0) {
		puts("Vector Arr is empty");
	}
	for(i32 i = 0; i < vec->length; i++) {
		printf("Token {.typ: %s; .dat:\"%s\"; ", tkToString[vec->toks[i].typ], vec->toks[i].data);
		if(vec->toks[i].typ == tk_fltLit) {
			printf(".val:%Lg ", vec->toks[i].Fval);
		} else if (vec->toks[i].typ == tk_intLit || vec->toks[i].typ == tk_chrLit) {
			printf(".val:%zu ", vec->toks[i].Ival);
		}
		
		printf("pos:%zu:%zu:%zu}\n", vec->toks[i].pos, vec->toks[i].ln_no, vec->toks[i].ln_index); 
	}
}

vectk Tokenizer(char* source, usize len) {
	Scanner scanner;
	vectk tokens;
	initScanner(&scanner, source, len);
	initVec(&tokens);

	while(*scanner.current != '\0') {
		char* errline = NULL;
		switch(*scanner.current) {
		case ' ':
		case '\t':
		case '\r':
			break;
		case '\n':
			scanner.ln_no++;
			scanner.ln_index = 0;
			scanner.line = scanner.current + 1;
			break;
		case '(':
			apndToken(&tokens, (struct Token){.typ = tk_lparen, .line = getstrLine(&scanner, errline), .data = tkVal[tk_lparen], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ')':
			apndToken(&tokens, (struct Token){.typ = tk_rparen, .line = getstrLine(&scanner, errline), .data = tkVal[tk_rparen], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '{':
			apndToken(&tokens, (struct Token){.typ = tk_lbrace, .line = getstrLine(&scanner, errline), .data = tkVal[tk_lbrace], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '}':
			apndToken(&tokens, (struct Token){.typ = tk_rbrace, .line = getstrLine(&scanner, errline), .data = tkVal[tk_rbrace], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '[':
			apndToken(&tokens, (struct Token){.typ = tk_lbracket, .line = getstrLine(&scanner, errline), .data = tkVal[tk_lbracket], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ']':
			apndToken(&tokens, (struct Token){.typ = tk_rbracket, .line = getstrLine(&scanner, errline), .data = tkVal[tk_rbracket], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '@':
			apndToken(&tokens, (struct Token){.typ = tk_addr, .line = getstrLine(&scanner, errline), .data = tkVal[tk_addr], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ',':
			apndToken(&tokens, (struct Token){.typ = tk_comma, .line = getstrLine(&scanner, errline), .data = tkVal[tk_comma], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '.':
			if(isNumb(*(scanner.current + 1))) {
				tokenizeNumbers(&scanner, &tokens);
				continue;
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_dot, .line = getstrLine(&scanner, errline), .data = tkVal[tk_dot], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case ';':
			apndToken(&tokens, (struct Token){.typ = tk_semi, .line = getstrLine(&scanner, errline), .data = tkVal[tk_semi], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '#':
			apndToken(&tokens, (struct Token){.typ = tk_hash, .line = getstrLine(&scanner, errline), .data = tkVal[tk_hash], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ':':
			apndToken(&tokens, (struct Token){.typ = tk_colon, .line = getstrLine(&scanner, errline), .data = tkVal[tk_colon], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '\\':
			apndToken(&tokens, (struct Token){.typ = tk_bslash, .line = getstrLine(&scanner, errline), .data = tkVal[tk_bslash], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		// case '$': --> idea: handle formatted strings with this symbol
		// 	puts("Error: '$' not implemented / used");
		// 	break; 
		// case '?': --> maybe for tenary exprs
		// 	puts("Error: '?' not implemented / used");
		// 	break;
		// case '~':
		// 	puts("Error: '~' not implemented / used");
		// 	break;
		// case '`':
		// 	puts("Error: '`' not implemented / used");
		// 	break;
		case '+': // ++ += +
			if(*(scanner.current + 1) == '+') { // tok ++
				apndToken(&tokens, (struct Token){.typ = tk_inc, .line = getstrLine(&scanner, errline), .data = tkVal[tk_inc], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') { // tok +=
				apndToken(&tokens, (struct Token){.typ = tk_addeq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_addeq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok +
				apndToken(&tokens, (struct Token){.typ = tk_add, .line = getstrLine(&scanner, errline), .data = tkVal[tk_add], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '-': // -- -= -> -
			if(*(scanner.current + 1) == '-') { // tok --
				apndToken(&tokens, (struct Token){.typ = tk_dec, .line = getstrLine(&scanner, errline), .data = tkVal[tk_dec], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') { // tok -=
				apndToken(&tokens, (struct Token){.typ = tk_subeq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_subeq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if(*(scanner.current + 1) == '>') { // tok ->
				apndToken(&tokens, (struct Token){.typ = tk_arrow, .line = getstrLine(&scanner, errline), .data = tkVal[tk_arrow], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok -
				apndToken(&tokens, (struct Token){.typ = tk_sub, .line = getstrLine(&scanner, errline), .data = tkVal[tk_sub], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '*': // *= *
			if(*(scanner.current + 1) == '=') { // tok *=
				apndToken(&tokens, (struct Token){.typ = tk_muleq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_muleq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if(*(scanner.current + 1) == '/') {
				char *endBl = malloc(3);
				endBl = "*/";
				errline = getstrLine(&scanner, errline);
				fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Ending of block comment found. \n%8zu | %s\n%8c |%*c\n", scanner.pos, scanner.ln_no, scanner.ln_index, scanner.ln_no, errline, ' ', (int)scanner.ln_index + 1, '^');
				apndToken(&tokens, (struct Token){.typ=tk_Error, .line=getstrLine(&scanner, errline), .data = endBl, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok *
				apndToken(&tokens, (struct Token){.typ = tk_mul, .line = getstrLine(&scanner, errline), .data = tkVal[tk_mul], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '/': // // /* */ /= / 
			if(*(scanner.current + 1) == '/') { // comment
				ignoreLineComment(&scanner);
				continue;
			} else if(*(scanner.current + 1) == '*') { /* block comment */
				// puts("Error: '/**/' not implemented");
				ignoreBlockComment(&scanner);
			} else if(*(scanner.current + 1) == '=') { // tok /=
				apndToken(&tokens, (struct Token){.typ = tk_diveq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_diveq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok /
				apndToken(&tokens, (struct Token){.typ = tk_div, .line = getstrLine(&scanner, errline), .data = tkVal[tk_div], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '%':
			if(*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_modeq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_modeq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_mod, .line = getstrLine(&scanner, errline), .data = tkVal[tk_mod], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '=': // == =
			if(*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_eqeqs, .line = getstrLine(&scanner, errline), .data = tkVal[tk_eqeqs], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_assign, .line = getstrLine(&scanner, errline), .data = tkVal[tk_assign], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '|': // || |
			if(*(scanner.current + 1) == '|') {
				apndToken(&tokens, (struct Token){.typ = tk_bitor, .line = getstrLine(&scanner, errline), .data = tkVal[tk_bitor], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_or, .line = getstrLine(&scanner, errline), .data = tkVal[tk_or], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '&': // && &
			if(*(scanner.current + 1) == '&') {
				apndToken(&tokens, (struct Token){.typ = tk_bitand, .line = getstrLine(&scanner, errline), .data = tkVal[tk_bitand], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_and, .line = getstrLine(&scanner, errline), .data = tkVal[tk_and], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '^': // ^^ ^
			if(*(scanner.current + 1) == '^') {
				apndToken(&tokens, (struct Token){.typ = tk_bitxor, .line = getstrLine(&scanner, errline), .data = tkVal[tk_bitxor], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_xor, .line = getstrLine(&scanner, errline), .data = tkVal[tk_xor], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '<':
			if(*(scanner.current + 1) == '<') {
				apndToken(&tokens, (struct Token){.typ = tk_shiftL, .line = getstrLine(&scanner, errline), .data = tkVal[tk_shiftL], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_lesseq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_lesseq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_less, .line = getstrLine(&scanner, errline), .data = tkVal[tk_less], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '!':
			if(*(scanner.current + 1) == '!') {
				apndToken(&tokens, (struct Token){.typ = tk_bitnot, .line = getstrLine(&scanner, errline), .data = tkVal[tk_bitnot], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_noteqs, .line = getstrLine(&scanner, errline), .data = tkVal[tk_noteqs], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_not, .line = getstrLine(&scanner, errline), .data = tkVal[tk_not], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '>':
			if(*(scanner.current + 1) == '>') {
				apndToken(&tokens, (struct Token){.typ = tk_shiftR, .line = getstrLine(&scanner, errline), .data = tkVal[tk_shiftR], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_greateq, .line = getstrLine(&scanner, errline), .data = tkVal[tk_greateq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_great, .line = getstrLine(&scanner, errline), .data = tkVal[tk_great], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '\"':
			puts("Error: '\"' not implemented");
			// AnalyzeStr(&scanner, &tokens);
			break;
		case '\'':
			puts("Error: '\'' not implemented");
			// AnalyzeChr(&scanner, &tokens);
			break;
		case 'a'...'z':
		case 'A'...'Z':
		case '_':
			tokenizeKeywordIndent(&scanner, &tokens);
			continue;
		case '0'...'9':
			tokenizeNumbers(&scanner, &tokens);
			continue;
		default:
			errline = getstrLine(&scanner, errline);
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Unknown Character: '%c' \n%8zu | %s\n%8c |%*c\n", scanner.pos, scanner.ln_no, scanner.ln_index, *scanner.current, scanner.ln_no, errline, ' ', (int)scanner.ln_index + 1, '^');
			// char ErrTok[2] = {*scanner.current, '\0'};
			char *ErrTok = malloc(2);
			ErrTok[0] = *scanner.current;
			ErrTok[1] = '\0';
			apndToken(&tokens, (struct Token){.typ=tk_Error, .line=errline, .data = ErrTok, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			scanner.isErr = true;
		}
		advance(&scanner);
	}
	char* errline = NULL;
	apndToken(&tokens, (struct Token){.typ=tk_EOF, .line=getstrLine(&scanner, errline), .data = tkVal[tk_EOF], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});

	// if(scanner.isErr) { exit(2); }
	// printTokens(&tokens);

	return tokens;
}
