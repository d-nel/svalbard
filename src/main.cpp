#include <stdio.h>

#include "parser.h"

int main(int, char **) {
	auto t = create_tokenizer("no_file", "if (banana == -1.0) { bum = 100 + -123 * 4.8; }");

	skip_whitespace(&t);
	auto token = next_token(&t);

	while (token.type != TOKEN_EOF) {
		printf("(%s) %s\n", Token_Type_Strings[token.type], token.value.data);
		destroy_string(&token.value);
		skip_whitespace(&t);
		token = next_token(&t);
	}

	destroy_string(&token.value);
	destroy_tokenizer(&t);

	return 0;
}
