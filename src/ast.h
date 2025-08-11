#ifndef ast_h
#define ast_h

#include "common.h"
#include "tok.h"

typedef struct AST AST;

struct AST {
	enum {
		AST_prog,
		AST_fndef,

		AST_retstmt,

		AST_intlit,
		AST_fltlit,

		AST_expr,
	} tag;

	union {
		struct AST_prog { AST* body; } AST_prog;
		struct AST_intlit { u64 ival; } AST_intlit;
		struct AST_fltlit { f128 fval; } AST_fltlit;
		struct AST_expr { AST * left; AST * right; enum { op_add, op_sub, op_mult, op_div, op_mod, } op; } AST_expr;
	} data;
};

AST* ast_new(AST ast);
#define AST_NEW(tag, ...) \
	ast_new((AST){tag, {.tag=(struct tag) {__VA_ARGS__}}})

#endif


// typedef enum {
// 	ast_Prog,
// 	ast_fndef,

// 	ast_retstmt,

// 	ast_intlit,
// 	ast_fltlit,

// 	ast_expr,

// } ASTtype;

// typedef enum {
// 	op_add, op_sub, op_mult, op_div, op_mod,
// 	op_eqeq, op_noteq, op_lt, op_lte, op_gt, op_gte,
// 	op_and, op_or, op_xor, op_not,
// 	op_bitand, op_bitor, op_bitxor, op_bitnot, op_shl, op_shr,
// } BinOpstyp;

// struct ASTNode; // forward declaration 

// struct scope {
// 	u64 stmtct;
// 	struct ASTNode *stmts;
// };

// struct fndef { // add default values
// 	char *name;
// 	Tokentyp rettyp;
// 	/*
// 	u32 paramCt; -> u8 paramCt
// 	u32 paramCapa; <- remove
// 	param* params; -> | params[127]
// 	*/
// 	struct scope body;
// };

// struct ASTNode {
// 	ASTtype typ;
// 	union {	
// 		char ident[256];

// 		u64 ival;
// 		f128 fval;

// 		struct {
// 			struct ASTNode *left;
// 			BinOpstyp op;
// 			struct ASTNode *right;
// 		} binexpr;

// 		struct {
// 			struct ASTNode *expr;
// 		} retstmt;

// 		struct fndef def;
// 	};
// };

// nil initAST(struct ASTNode *root);





// struct Const {
// 	Tokentyp typ;
// 	union {
// 		u64 ival;
// 		f128 fval;
// 	};
// };

// struct node {
// 	enum {
// 		nd_fndef,
// 		nd_vardef,
// 	} nodetyp;

// 	void* child;
// };

// struct scope {
// 	u64 ScopeID;

// 	struct node* stmts;
// 	u32 len;
// 	u32 capacity;
// };

// struct expr {
// 	enum {
// 		expr_const,
// 	} exprtyp;

// 	void* child;
// };

// struct param {
// 	Tokentyp datatyp;
// 	char* identifier;

// 	// add default value expr
// };

// struct fndef {
// 	char* identifier;
// 	Tokentyp rettyp;
// 	struct param* params;
// 	struct scope block;
// };

// struct retstmt {
// 	struct expr expression;
// };

// void initAST(struct scope* ast);