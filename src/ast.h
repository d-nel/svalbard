#pragma once

#include "types.h"
#include "dan_string.h"

struct Location {
	u64 l0, c0, l1, c1 = 0;
};

enum AstType {
	AST_BASE,
	AST_EXPRESSION,
	AST_STATEMENT,

	AST_OPERATOR,
	
	AST_LITERAL,

	AST_COUNT
};

static const char *AstType_Strings[AST_COUNT] = {
	"AST_BASE",
	"AST_EXPRESSION",
	"AST_STATEMENT",
	
	"AST_OPERATOR",

	"AST_LITERAL"
};

enum ExpressionType {
	EXP_UNKNOWN,

	EXP_POINTER,
	EXP_STRUCT,

	EXP_U8,
	EXP_U16,
	EXP_U32,
	EXP_U64,

	EXP_S8,
	EXP_S16,
	EXP_S32,
	EXP_S64,

	EXP_F32,
	EXP_F64,

	EXP_BOOL,

	EXP_COUNT
};

enum OperatorType {
	OP_INVALID,
	OP_MUL,
	OP_DIV,
	OP_ADD,
	OP_SUB,

	OP_COUNT
};

struct Ast_Base {
	AstType ast_type = AST_BASE;

	Location loc;
};

struct Ast_Expression : Ast_Base {
	Ast_Expression() { ast_type = AST_EXPRESSION; }

	ExpressionType type;
	u64 size;
	bool is_literal;
	
	union {
		u8  _u8;
		u16 _u16;
		u32 _u32;
		u64 _u64;

		s8  _s8;
		s16 _s16;
		s32 _s32;
		s64 _s64;

		f32 _f32;
		f64 _f64;

		bool _bool;
	} literal_value;

	// EXP_POINTER
	Ast_Expression *pointing_to;
};

struct Ast_Operator : Ast_Expression {
	Ast_Operator() { ast_type = AST_OPERATOR; }

	OperatorType type = OP_INVALID;
	Ast_Expression *lhs;
	Ast_Expression *rhs;
};
