#include <stdio.h>

#include "parser.h"

void print_ast(Ast_Expression *expr) {
	if (expr->ast_type == AST_OPERATOR) {
		auto op = (Ast_Operator *)(expr);
		printf("(");
		print_ast(op->lhs);

		if (op->type == OP_MUL) {
			printf(" * ");
		} else if (op->type == OP_ADD) {
			printf(" + ");
		} else if (op->type == OP_INVALID) {
			printf(" ? ");
		}

		print_ast(op->rhs);
		printf(")");
	} else if (expr->is_literal) {

		if (expr->type == EXP_F64) {
			printf("%f", expr->literal_value._f64);
		} else if (expr->type == EXP_U64) {
			printf("%llu", expr->literal_value._u64);
		} else if (expr->type == EXP_S64) {
			printf("%lld", expr->literal_value._s64);
		}
	}

}

int main(int, char **) {
	Parser p = create_parser("test.sva");

	auto node = (Ast_Expression *)next_node(&p);


	print_ast(node);
	printf("\n");

	destroy_parser(&p);

	return 0;
}
