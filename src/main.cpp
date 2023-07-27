#include <stdio.h>

#include "parser.h"

int main(int, char **) {
	Parser p = create_parser("test.sva");

	print_all_tokens_until_eof(&p);

	destroy_parser(&p);

	return 0;
}
