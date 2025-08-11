#include "ast.h"

AST* ast_new(AST ast) {
	AST *tmp = malloc(sizeof(AST));
	if(tmp)
		*tmp = ast;
	return tmp;
}

nil ast_free(AST *ptr) {
	AST ast = *ptr;

	switch(ast.tag) {
		case AST_prog:
			struct AST_prog data = ast.data.AST_prog;
			ast_free(data.body);
			break;
		case AST_fndef:
			break;
		case AST_retstmt:
			break;
		case AST_intlit:
			break;
		case AST_fltlit:
			break;
		case AST_expr:
			break;
	}

	free(ptr);
}