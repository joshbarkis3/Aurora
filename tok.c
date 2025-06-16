#include "tok.h"
#include "memory.h"

#define IndentMaxSize ((usize)255)
#define NumberMaxSize ((usize)255)

char* tkVal[TKCOUNT] = {
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
	[tk_lbracket] = "[",
	[tk_rbracket] = "]",
	[tk_comma] = ",",
	[tk_dot] = ".",
	[tk_arrow] = "->",
	[tk_colon] = ":",
	[tk_semi] = ";",
	[tk_addr] = "@",
};

// cool hack to get around using a ToString fn
// and a large switch case
const char* tkToString[TKCOUNT] = {
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

static void initScanner(Scanner* scanner, char* source, usize len) {
	scanner->source = source;
	scanner->current = source;
	scanner->line = source;

	scanner->src_len = len;

	scanner->pos = 0;
	scanner->ln_no = 1;
	scanner->ln_index = 1;
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

static void freeVec(vectk* vec) {
	free(vec);
	initVec(vec);
}

static void advance(Scanner* scanner) {
	scanner->current++;
	scanner->pos++;
	scanner->ln_index++;
}

static bool isalpha(char ch) {
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z';
}

static bool isNumb(char ch) {
	return ('0' <= ch && ch <= '9');
}

static void tokenizeKeywordIndent(Scanner* scanner, vectk* vec) {
	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
	str s;
	StrInit(&s, "", IndentMaxSize);
	while(isalpha(*scanner->current) || isNumb(*scanner->current) || *scanner->current == '_') {
		if(s.len == s.MaxSz) {
			fprintf(stderr, "Indentifier/Keywords have a max size of 127 chars @ %zu:%zu:%zu", pos, ln_no, column);
			exit(2);
		}

		StrChrCat(&s, *scanner->current);
		advance(scanner);
	}

	if(StrcCmp(&s, "fn")) {
		apndToken(vec, (struct Token) {.typ = tk_fn, .line = NULL, .data = tkVal[tk_fn], .val = 0, .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "i32") || StrcCmp(&s, "int32") || StrcCmp(&s, "int")) { 
		apndToken(vec, (struct Token) {.typ = tk_int32, .line = NULL, .data = tkVal[tk_int32], .val = 0, .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "return")) {
		apndToken(vec, (struct Token) {.typ = tk_ret, .line = NULL, .data = tkVal[tk_ret], .val = 0, .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "nil")) {
		apndToken(vec, (struct Token) {.typ = tk_nil, .line = NULL, .data = tkVal[tk_nil], .val = 0, .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else {
		apndToken(vec, (struct Token){.typ=tk_Ident, .line = NULL, .data=s.string, .val = 0, .pos = pos, .ln_no = ln_no, .ln_index = column});
	}

	return;
}

static void tokenizeNumbers(Scanner* scanner, vectk* vec) {
	u64 pos = scanner->pos, ln_no = scanner->ln_no, column = scanner->ln_index;
	bool hasDot;
	bool isExp;
	str s;
	StrInit(&s, "", NumberMaxSize);
	while(isNumb(*scanner->current) || *scanner->current == '.' || *scanner->current == 'e' || *scanner->current == 'E') {
		if(s.len == s.MaxSz) {
			fprintf(stderr, "Numbers have a max size of 127 chars @ %zu:%zu:%zu", pos, ln_no, column);
			exit(2);
		}

		if(*scanner->current == '.') {
			hasDot = true;
		} else if (*scanner->current == 'e' || *scanner->current == 'E') {
			isExp = true;
		}

		StrChrCat(&s, *scanner->current);
		advance(scanner);
	}

	if (hasDot || isExp) {
		apndToken(vec, (struct Token){.typ=tk_Lit, .line = NULL, .data=s.string, .val = atof(s.string), .pos = pos, .ln_no = ln_no, .ln_index = column});	
	} else {
		apndToken(vec, (struct Token){.typ=tk_Lit, .line = NULL, .data=s.string, .val = atoi(s.string), .pos = pos, .ln_no = ln_no, .ln_index = column});	
	}

	return;
}

void printTokens(vectk* vec) {
	if (vec->length <= 0) {
		puts("Vector Arr is empty");
	}
	for(i32 i = 0; i < vec->length; i++) {
		printf("Token {.typ: %s; .dat:\"%s\"; .val:%zu pos:%zu:%zu:%zu}\n", 
			tkToString[vec->toks[i].typ], vec->toks[i].data, vec->toks[i].val, 
					vec->toks[i].pos, vec->toks[i].ln_no, vec->toks[i].ln_index);
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
		case '[':
			apndToken(&tokens, (struct Token){.typ = tk_lbracket, .line = NULL, .data = tkVal[tk_lbracket], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case ']':
			apndToken(&tokens, (struct Token){.typ = tk_rbracket, .line = NULL, .data = tkVal[tk_rbracket], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		// + - * / % " ' & | ^ ! = \ < >
		case '@':
			puts("Error: '@' not implemented");
			break;
		case ',':
			puts("Error: ',' not implemented");
			break;
		case '.':
			if(isNumb(*(scanner.current + 1))) {
				tokenizeNumbers(&scanner, &tokens);
				continue;
			} else {
				puts("Error: '.' not implemented");	
			}
			break;
		case ';':
			apndToken(&tokens, (struct Token){.typ = tk_semi, .line = NULL, .data = tkVal[tk_semi], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			break;
		case '#':
			puts("Error: '#' not implemented");
			break;
		case ':':
			puts("Error: ':' not implemented");
			break;
		case '\\':
			puts("Error: '\\' not implemented");
			break;
		case '$':
			puts("Error: '$' not implemented / used");
			break;
		case '?':
			puts("Error: '?' not implemented / used");
			break;
		case '~':
			puts("Error: '~' not implemented / used");
			break;
		case '`':
			puts("Error: '`' not implemented / used");
			break;
		// -> ++ -- // /* */ += -= /= *= %= == != <= >= || && ^^ !!<< >>
		// maybe: $ ? ~ `
		case '+': // ++ += +
			if(*(scanner.current + 1) == '+') {
				// tok ++
				puts("Error: '++' not implemented");
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				// tok +=
				puts("Error: '+=' not implemented");
				advance(&scanner);
			} else {
				// tok +
				puts("Error: '+' not implemented");
			}
			break;
		case '-': // -- -= -> -
			if(*(scanner.current + 1) == '-') {
				// tok --
				puts("Error: '--' not implemented");
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				// tok -=
				puts("Error: '-=' not implemented");
				advance(&scanner);
			} else if(*(scanner.current + 1) == '>') {
				// tok ->
				apndToken(&tokens, (struct Token){.typ = tk_arrow, .line = NULL, .data = tkVal[tk_arrow], .val = 0, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else {
				// tok -
				puts("Error: '-' not implemented");
			}
			break;
		case '*': // *= *
			if(*(scanner.current + 1) == '=') {
				// tok *=
				puts("Error: '*=' not implemented");
				advance(&scanner);
			} else {
				// tok *
				puts("Error: '*' not implemented");
			}
			break;
		case '/': // // /* */ /= / 
			if(*(scanner.current + 1) == '/') {
				// comment
				puts("Error: '//' not implemented");
				advance(&scanner);
			} else if(*(scanner.current + 1) == '*') {
				// block comment
				puts("Error: '/**/' not implemented");
				advance(&scanner);
			} else if(*(scanner.current + 1) == '=') {
				// tok /=
				puts("Error: '/=' not implemented");
				advance(&scanner);
			} else {
				// tok /
				puts("Error: '/' not implemented");
			}
			break;
		case '%':
			if(*(scanner.current + 1) == '=') {
				puts("Error: '%=' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '%' not implemented");
			}
			break;
		case '=': // == =
			if(*(scanner.current + 1) == '=') {
				puts("Error: '==' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '=' not implemented");
			}
			break;
		case '|': // || |
			if(*(scanner.current + 1) == '|') {
				puts("Error: '||' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '|' not implemented");
			}
			break;
		case '&': // && &
			if(*(scanner.current + 1) == '&') {
				puts("Error: '&&' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '&' not implemented");
			}
			break;
		case '^': // ^^ ^
			if(*(scanner.current + 1) == '^') {
				puts("Error: '^^' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '^' not implemented");
			}
			break;
		case '<':
			if(*(scanner.current + 1) == '<') {
				puts("Error: '<<' not implemented");
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				puts("Error: '<=' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '<' not implemented");
			}
			break;
		case '!':
			if(*(scanner.current + 1) == '!') {
				puts("Error: '!!' not implemented");
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				puts("Error: '!=' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '!' not implemented");
			}
			break;
		case '>':
			if(*(scanner.current + 1) == '>') {
				puts("Error: '>>' not implemented");
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') {
				puts("Error: '>=' not implemented");
				advance(&scanner);
			} else {
				puts("Error: '>' not implemented");
			}
			break;
		case '\"':
			puts("Error: '\"' not implemented");
			break;
		case '\'':
			puts("Error: '\'' not implemented");
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
			fprintf(stderr, "Error Unknown Character identified: '%c' @ %zu:%zu:%zu\n", *scanner.current, scanner.pos, scanner.ln_no, scanner.ln_index);
			// exit(2);
		}
		advance(&scanner);
	}

	printTokens(&tokens);

	return tokens;
}
