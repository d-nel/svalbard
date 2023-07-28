#include <stdio.h>

#include "parser.h"


int main(int, char **) {
	Parser p = create_parser("test.sva");

	auto node = (Ast_Expression *)next_node(&p);

	print_node(node);

	destroy_parser(&p);

	return 0;
}
