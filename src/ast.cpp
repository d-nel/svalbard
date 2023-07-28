#include "stdio.h"

#include "ast.h"

void print_node(Ast_Base *node, s32 level) {
	printf("%*s%s", level * 2, "", "↳");

	if (node->ast_type == AST_EXPRESSION) {
		auto expr = (Ast_Expression *)node;
		if (expr->is_literal) {
			if (expr->type == EXP_F64) {
				printf("F64 %f\n", expr->literal_value._f64);
			} else if (expr->type == EXP_U64) {
				printf("U64 %llu\n", expr->literal_value._u64);
			} else if (expr->type == EXP_S64) {
				printf("S64 %lld\n", expr->literal_value._s64);
			} else if (expr->type == EXP_BOOL) {
				if (expr->literal_value._bool) printf("BOOL true\n");
				else printf("BOOL false\n");
			} else {
				printf("AST_EXPRESSION(is_literal)");
			}
		} else {
			printf("AST_EXPRESSION");
		}
	} else if (node->ast_type == AST_OPERATOR) {
		auto op = (Ast_Operator *)node;
		printf("Operator %s\n", OperatorType_Strings[op->type]);

		if (op->lhs) print_node(op->lhs, level+1);
		if (op->rhs) print_node(op->rhs, level+1);
	} else {
		printf("AST_BASE");
	}
}
