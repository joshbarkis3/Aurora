#include "parser.h"

// void initParser(struct Parser* p, const vectk* tk) {
// 	p->toks = tk;
// 	p->curr = p->toks->toks;
// 	p->tkCt = p->toks->length;
// 	p->pos = 0;
// }

// void advance(struct Parser* p) {
// 	printf("%d : %d\n", p->pos, p->tkCt);
// 	if(p->pos + 1 >= p->tkCt) {
// 		p->curr = p->toks->toks + p->tkCt - 1;
// 		// printf("too many tokens\n");
// 	} else {
// 		// printf("tokens\n");
// 		p->curr++;
// 		p->pos++;
// 	}
// }

// struct Token peek(struct Parser* p, i32 offset) {
// 	if(p->pos + offset >= p->tkCt) {
// 		return *(p->curr + offset);
// 	}
// 	return p->toks->toks[p->tkCt - 1];
// }

// void expect(struct Parser* p, Tokentyp type) {
// 	if(p->curr->typ == type) {
// 		puts("WOOOOOOOOOOOOOOOOOOOOOOOOOOO");
// 	} else {
// 		fprintf(stderr, "Expected token \"%s\" got \"%s\"\n", tkVal[type], p->curr->typ > 9 ? tkVal[p->curr->typ] : tkToString[p->curr->typ]);
// 	}
// 	advance(p);
// }

// void parse_fn(struct Parser* p, struct ASTNode* node) {
// 	// char identname[255] = {0};
// 	expect(p, tk_fn);
// 	if(p->curr->typ == tk_Ident) {
// 		strncpy(node->def.name, p->curr->data, 255);
// 		node->def.name[256] = '\0';
// 		advance(p);
// 	} else {
// 		// error
// 	}
// 	expect(p, tk_lparen);
// 	expect(p, tk_nil); // replace here with parse parameters
// 	expect(p, tk_rparen);
// 	expect(p, tk_arrow);
// 	if(p->curr->typ >= tk_int8 && p->curr->typ <= tk_ptr) {
// 		node->def.rettyp = p->curr->typ;
// 	} else {
// 		// error
// 	}
// 	expect(p, tk_lbrace);


	
	
// }

// void parse(const vectk* tk) {
// 	struct Parser p;
// 	struct ASTNode Prog;

// 	initParser(&p, tk);
// 	initAST(&Prog);

// 	printf("%d\n", p.tkCt);

// 	if(p.curr->typ == tk_fn) {
// 		parse_fn(&p, &Prog);
// 	}	


// 	expect(&p, tk_fn);
// 	expect(&p, tk_EOF);
// 	expect(&p, tk_EOF);
// 	expect(&p, tk_EOF);
// 	exit(212);	
// }
