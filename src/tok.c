#include "tok.h"
#include "memory.h"

#define IndentMaxSize ((usize) 255)
#define NumberMaxSize ((usize) 255)

char* tkVal[TKCOUNT] = {
	[tk_Error] = 0,
	[tk_EOF] = 0,
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
			free(vec->toks[i].data);
		}
	}
	free(vec->toks);
	initVec(vec);
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

static char* getstrLine(Scanner* scanner) {
	str line;
	StrInit(&line, "", 0);

	i32 i = 0;
	while(*(scanner->line + i) != '\n') {
		if(*(scanner->line + i) != '\r') {
			StrChrCat(&line, *(scanner->line + i));
		}
		i++;
	}
	
	return line.string;
}

void handleInvalidIndent(Scanner* scanner, str *s, vectk* vec, u64 pos, u64 ln_no, u64 column) {
	while(isalph(*scanner->current) || isNumb(*scanner->current)) {
		StrChrCat(s, *scanner->current);
		advance(scanner);
	}
	fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Invalid Identifier name: \"%s\"\n%8zu | %s\n%8c |%*c\n", pos, ln_no, column, s->string, ln_no, getstrLine(scanner), ' ', (int)column + 1, '^');
	apndToken(vec, (struct Token){.typ=tk_Error, .line=NULL, .data = s->string, .pos = pos, .ln_no = ln_no, .ln_index = column});
	scanner->isErr = true;
}

/*
helper function to tokenize identifiers or keywords.
Memory Leaks may be found here -- I did try to clean them up
*/
static void tokenizeKeywordIndent(Scanner* scanner, vectk* vec) {
	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
	str s;
	StrInit(&s, "", IndentMaxSize);
	while(isalph(*scanner->current) || isNumb(*scanner->current) || *scanner->current == '_') {
		if(s.len == s.MaxSz) {
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Indentifier/Keywords have a max size of 255 chars", pos, ln_no, column);
			exit(2);
		}

		StrChrCat(&s, *scanner->current);
		advance(scanner);
	}

	if(StrcCmp(&s, "fn")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_fn, .line = NULL, .data = tkVal[tk_fn], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "return")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_ret, .line = NULL, .data = tkVal[tk_ret], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "break")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_break, .line = NULL, .data = tkVal[tk_break], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "continue")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_cont, .line = NULL, .data = tkVal[tk_cont], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "do")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_do, .line = NULL, .data = tkVal[tk_do], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "while")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_while, .line = NULL, .data = tkVal[tk_while], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "for")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_for, .line = NULL, .data = tkVal[tk_for], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "if")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_if, .line = NULL, .data = tkVal[tk_if], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "else")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_else, .line = NULL, .data = tkVal[tk_else], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "goto")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_goto, .line = NULL, .data = tkVal[tk_goto], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "switch")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_switch, .line = NULL, .data = tkVal[tk_switch], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "case")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_case, .line = NULL, .data = tkVal[tk_case], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "default")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_default, .line = NULL, .data = tkVal[tk_default], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "auto")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_auto, .line = NULL, .data = tkVal[tk_auto], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "import")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_import, .line = NULL, .data = tkVal[tk_import], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "use")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_use, .line = NULL, .data = tkVal[tk_use], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "typedef")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_typedef, .line = NULL, .data = tkVal[tk_typedef], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "i8") || StrcCmp(&s, "int8") || StrcCmp(&s, "sbyte")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_int8, .line = NULL, .data = tkVal[tk_int8], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "i16") || StrcCmp(&s, "int16") || StrcCmp(&s, "short")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_int16, .line = NULL, .data = tkVal[tk_int16], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "i32") || StrcCmp(&s, "int32") || StrcCmp(&s, "int")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_int32, .line = NULL, .data = tkVal[tk_int32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "i64") || StrcCmp(&s, "int64") || StrcCmp(&s, "long") || StrcCmp(&s, "isize")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_int64, .line = NULL, .data = tkVal[tk_int64], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "u1") || StrcCmp(&s, "uint1") || StrcCmp(&s, "bool")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_uint1, .line = NULL, .data = tkVal[tk_uint1], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "u8") || StrcCmp(&s, "uint8") || StrcCmp(&s, "byte") || StrcCmp(&s, "char")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_uint8, .line = NULL, .data = tkVal[tk_uint8], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "u16") || StrcCmp(&s, "uint16") || StrcCmp(&s, "ushort")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_uint16, .line = NULL, .data = tkVal[tk_uint16], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "u32") || StrcCmp(&s, "uint32") || StrcCmp(&s, "uint")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_uint32, .line = NULL, .data = tkVal[tk_uint32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "u64") || StrcCmp(&s, "uint64") || StrcCmp(&s, "ulong") || StrcCmp(&s, "usize")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_uint64, .line = NULL, .data = tkVal[tk_uint64], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "f32") || StrcCmp(&s, "float32") || StrcCmp(&s, "float")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_float32, .line = NULL, .data = tkVal[tk_float32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "f64") || StrcCmp(&s, "float64") || StrcCmp(&s, "double")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_float64, .line = NULL, .data = tkVal[tk_float64], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "f128") || StrcCmp(&s, "float128") || StrcCmp(&s, "ldouble")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_float128, .line = NULL, .data = tkVal[tk_float128], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "nil") || StrcCmp(&s, "u0")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_nil, .line = NULL, .data = tkVal[tk_nil], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "str")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_str, .line = NULL, .data = tkVal[tk_str], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "ptr")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_ptr, .line = NULL, .data = tkVal[tk_ptr], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else {
		apndToken(vec, (struct Token){.typ=tk_Ident, .line = NULL, .data = s.string, .pos = pos, .ln_no = ln_no, .ln_index = column});
	}

	// free(s.string); // memory leakers

	return;
}

static void tokenizeNumbers(Scanner* scanner, vectk* vec) {
	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
	bool hasDot = false;
	bool isExp = false;
	bool isErr = false;
	str s;
	StrInit(&s, "", NumberMaxSize);
	while(isNumb(*scanner->current) || *scanner->current == '.' || *scanner->current == 'e' || *scanner->current == 'E') {
		if(s.len == s.MaxSz) {
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Numbers have a max size of 255 chars\n", pos, ln_no, column);
			exit(2);
		}

		if(*scanner->current == '.') {
			if(hasDot) {
				if(!isErr && hasDot)
					fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Invalid Suffix for float \n%8zu | %s\n%8c |%*c\n", pos, ln_no, column, ln_no, getstrLine(scanner), ' ', (int)scanner->ln_index + 1, '^');
				scanner->isErr = true;
				isErr = true;
			} else {
				hasDot = true;	
			}
		} 
		if ((*scanner->current == 'e' || *scanner->current == 'E')) {
			if(isExp) {
				if(!isErr && isExp)
					fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Floats cannot have more than one exp\n%8zu | %s\n%8c |%*c\n", pos, ln_no, column, ln_no, getstrLine(scanner), ' ', (int)scanner->ln_index + 1, '^');
				scanner->isErr = true;
				isErr = true;
			} else {
				isExp = true;	
			}

			if (*(scanner->current + 1) == '-') {
				StrChrCat(&s, *scanner->current);
				advance(scanner);
			}
		}

		StrChrCat(&s, *scanner->current);
		advance(scanner);

		if (isalph(*scanner->current) && !(*scanner->current == 'e' || *scanner->current == 'E')) {
			handleInvalidIndent(scanner, &s, vec, pos, ln_no, column);
			return;
		}
	}

	if ((hasDot || isExp) && !isErr) {
		apndToken(vec, (struct Token){.typ=tk_fltLit, .line = NULL, .data=s.string, .Fval = atof(s.string), .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (!hasDot && !isExp && !isErr) {
		apndToken(vec, (struct Token){.typ=tk_intLit, .line = NULL, .data=s.string, .Ival = atoll(s.string), .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else {
		apndToken(vec, (struct Token){.typ=tk_Error, .line = NULL, .data=s.string, .pos = pos, .ln_no = ln_no, .ln_index = column});
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
			apndToken(&tokens, (struct Token){.typ = tk_lparen, .line = NULL, .data = tkVal[tk_lparen], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ')':
			apndToken(&tokens, (struct Token){.typ = tk_rparen, .line = NULL, .data = tkVal[tk_rparen], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '{':
			apndToken(&tokens, (struct Token){.typ = tk_lbrace, .line = NULL, .data = tkVal[tk_lbrace], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '}':
			apndToken(&tokens, (struct Token){.typ = tk_rbrace, .line = NULL, .data = tkVal[tk_rbrace], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '[':
			apndToken(&tokens, (struct Token){.typ = tk_lbracket, .line = NULL, .data = tkVal[tk_lbracket], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ']':
			apndToken(&tokens, (struct Token){.typ = tk_rbracket, .line = NULL, .data = tkVal[tk_rbracket], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '@':
			apndToken(&tokens, (struct Token){.typ = tk_addr, .line = NULL, .data = tkVal[tk_addr], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ',':
			apndToken(&tokens, (struct Token){.typ = tk_comma, .line = NULL, .data = tkVal[tk_comma], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '.':
			if(isNumb(*(scanner.current + 1))) {
				tokenizeNumbers(&scanner, &tokens);
				continue;
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_dot, .line = NULL, .data = tkVal[tk_dot], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case ';':
			apndToken(&tokens, (struct Token){.typ = tk_semi, .line = NULL, .data = tkVal[tk_semi], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '#':
			apndToken(&tokens, (struct Token){.typ = tk_hash, .line = NULL, .data = tkVal[tk_hash], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ':':
			apndToken(&tokens, (struct Token){.typ = tk_colon, .line = NULL, .data = tkVal[tk_colon], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '\\':
			apndToken(&tokens, (struct Token){.typ = tk_bslash, .line = NULL, .data = tkVal[tk_bslash], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
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
				apndToken(&tokens, (struct Token){.typ = tk_inc, .line = NULL, .data = tkVal[tk_inc], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') { // tok +=
				apndToken(&tokens, (struct Token){.typ = tk_addeq, .line = NULL, .data = tkVal[tk_addeq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok +
				apndToken(&tokens, (struct Token){.typ = tk_add, .line = NULL, .data = tkVal[tk_add], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '-': // -- -= -> -
			if(*(scanner.current + 1) == '-') { // tok --
				apndToken(&tokens, (struct Token){.typ = tk_dec, .line = NULL, .data = tkVal[tk_dec], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') { // tok -=
				apndToken(&tokens, (struct Token){.typ = tk_subeq, .line = NULL, .data = tkVal[tk_subeq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if(*(scanner.current + 1) == '>') { // tok ->
				apndToken(&tokens, (struct Token){.typ = tk_arrow, .line = NULL, .data = tkVal[tk_arrow], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok -
				apndToken(&tokens, (struct Token){.typ = tk_sub, .line = NULL, .data = tkVal[tk_sub], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '*': // *= *
			if(*(scanner.current + 1) == '=') { // tok *=
				apndToken(&tokens, (struct Token){.typ = tk_muleq, .line = NULL, .data = tkVal[tk_muleq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if(*(scanner.current + 1) == '/') {
				char *endBl = malloc(3);
				endBl = "*/";
				fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Ending of block comment found. \n%8zu | %s\n%8c |%*c\n", scanner.pos, scanner.ln_no, scanner.ln_index, scanner.ln_no, getstrLine(&scanner), ' ', (int)scanner.ln_index + 1, '^');
				apndToken(&tokens, (struct Token){.typ=tk_Error, .line=NULL, .data = endBl, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok *
				apndToken(&tokens, (struct Token){.typ = tk_mul, .line = NULL, .data = tkVal[tk_mul], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
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
				apndToken(&tokens, (struct Token){.typ = tk_diveq, .line = NULL, .data = tkVal[tk_diveq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok /
				apndToken(&tokens, (struct Token){.typ = tk_div, .line = NULL, .data = tkVal[tk_div], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '%':
			if(*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_modeq, .line = NULL, .data = tkVal[tk_modeq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_mod, .line = NULL, .data = tkVal[tk_mod], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '=': // == =
			if(*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_eqeqs, .line = NULL, .data = tkVal[tk_eqeqs], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_assign, .line = NULL, .data = tkVal[tk_assign], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '|': // || |
			if(*(scanner.current + 1) == '|') {
				apndToken(&tokens, (struct Token){.typ = tk_bitor, .line = NULL, .data = tkVal[tk_bitor], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_or, .line = NULL, .data = tkVal[tk_or], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '&': // && &
			if(*(scanner.current + 1) == '&') {
				apndToken(&tokens, (struct Token){.typ = tk_bitand, .line = NULL, .data = tkVal[tk_bitand], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_and, .line = NULL, .data = tkVal[tk_and], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '^': // ^^ ^
			if(*(scanner.current + 1) == '^') {
				apndToken(&tokens, (struct Token){.typ = tk_bitxor, .line = NULL, .data = tkVal[tk_bitxor], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_xor, .line = NULL, .data = tkVal[tk_xor], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '<':
			if(*(scanner.current + 1) == '<') {
				apndToken(&tokens, (struct Token){.typ = tk_shiftL, .line = NULL, .data = tkVal[tk_shiftL], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_lesseq, .line = NULL, .data = tkVal[tk_lesseq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_less, .line = NULL, .data = tkVal[tk_less], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '!':
			if(*(scanner.current + 1) == '!') {
				apndToken(&tokens, (struct Token){.typ = tk_bitnot, .line = NULL, .data = tkVal[tk_bitnot], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_noteqs, .line = NULL, .data = tkVal[tk_noteqs], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_not, .line = NULL, .data = tkVal[tk_not], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			}
			break;
		case '>':
			if(*(scanner.current + 1) == '>') {
				apndToken(&tokens, (struct Token){.typ = tk_shiftR, .line = NULL, .data = tkVal[tk_shiftR], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				apndToken(&tokens, (struct Token){.typ = tk_greateq, .line = NULL, .data = tkVal[tk_greateq], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				apndToken(&tokens, (struct Token){.typ = tk_great, .line = NULL, .data = tkVal[tk_great], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
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
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Unknown Character: '%c' \n%8zu | %s\n%8c |%*c\n", scanner.pos, scanner.ln_no, scanner.ln_index, *scanner.current, scanner.ln_no, getstrLine(&scanner), ' ', (int)scanner.ln_index + 1, '^');
			// char ErrTok[2] = {*scanner.current, '\0'};
			char *ErrTok = malloc(2);
			ErrTok[0] = *scanner.current;
			ErrTok[1] = '\0';
			apndToken(&tokens, (struct Token){.typ=tk_Error, .line=NULL, .data = ErrTok, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			scanner.isErr = true;
		}
		advance(&scanner);
	}

	// if(scanner.isErr) { exit(2); }
	// printTokens(&tokens);

	return tokens;
}
