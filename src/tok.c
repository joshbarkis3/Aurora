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
		if(vec->toks[i].typ == tk_Ident || vec->toks[i].typ == tk_fltLit || vec->toks[i].typ == tk_intLit || vec->toks[i].typ == tk_strLit) {
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
	} else if (StrcCmp(&s, "i32") || StrcCmp(&s, "int32") || StrcCmp(&s, "int")) { 
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_int32, .line = NULL, .data = tkVal[tk_int32], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "return")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_ret, .line = NULL, .data = tkVal[tk_ret], .pos = pos, .ln_no = ln_no, .ln_index = column});
	} else if (StrcCmp(&s, "nil")) {
		free(s.string);
		apndToken(vec, (struct Token) {.typ = tk_nil, .line = NULL, .data = tkVal[tk_nil], .pos = pos, .ln_no = ln_no, .ln_index = column});
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

// static void ignoreComment(Scanner* scanner) {
// 	// todo
// }

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
			apndToken(&tokens, (struct Token){.typ = tk_semi, .line = NULL, .data = tkVal[tk_semi], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
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
		// case '$':
		// 	puts("Error: '$' not implemented / used");
		// 	break;
		// case '?':
		// 	puts("Error: '?' not implemented / used");
		// 	break;
		// case '~':
		// 	puts("Error: '~' not implemented / used");
		// 	break;
		// case '`':
		// 	puts("Error: '`' not implemented / used");
		// 	break;
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
			if(*(scanner.current + 1) == '-') { // tok --
				puts("Error: '--' not implemented");
				advance(&scanner);
			} else if (*(scanner.current + 1) == '=') { // tok -=
				puts("Error: '-=' not implemented");
				advance(&scanner);
			} else if(*(scanner.current + 1) == '>') { // tok ->
				apndToken(&tokens, (struct Token){.typ = tk_arrow, .line = NULL, .data = tkVal[tk_arrow], .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
				advance(&scanner);
			} else { // tok -
				puts("Error: '-' not implemented");
			}
			break;
		case '*': // *= *
			if(*(scanner.current + 1) == '=') { // tok *=
				puts("Error: '*=' not implemented");
				advance(&scanner);
			} else { // tok *
				puts("Error: '*' not implemented");
			}
			break;
		case '/': // // /* */ /= / 
			if(*(scanner.current + 1) == '/') { // comment
				puts("Error: '//' not implemented");
				advance(&scanner);
			} else if(*(scanner.current + 1) == '*') { /* block comment */
				puts("Error: '/**/' not implemented");
				advance(&scanner);
			} else if(*(scanner.current + 1) == '=') { // tok /=
				puts("Error: '/=' not implemented");
				advance(&scanner);
			} else { // tok /
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
			fprintf(stderr, "Tokenizer Error @ %zu:%zu:%zu: Unknown Character identified: '%c' \n%8zu | %s\n%8c |%*c\n", scanner.pos, scanner.ln_no, scanner.ln_index, *scanner.current, scanner.ln_no, getstrLine(&scanner), ' ', (int)scanner.ln_index + 1, '^');
			char ErrTok[2] = {*scanner.current, '\0'};
			apndToken(&tokens, (struct Token){.typ=tk_Error, .line=NULL, .data = ErrTok, .pos = scanner.pos, .ln_no = scanner.ln_no, .ln_index = scanner.ln_index});
			scanner.isErr = true;
		}
		advance(&scanner);
	}

	if(scanner.isErr) { exit(2); }
	// printTokens(&tokens);

	return tokens;
}
