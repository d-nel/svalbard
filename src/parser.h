#pragma once

#include "types.h"
#include "dan_string.h"


struct Location {
	u64 l0, c0, l1, c1 = 0;
};

enum TokenType {
	TOKEN_SYMBOL,
	TOKEN_IDENTIFIER,

	TOKEN_LITERAL_BOOL,
	TOKEN_LITERAL_STRING,
	TOKEN_LITERAL_FLOAT,
	TOKEN_LITERAL_UINTEGER,
	TOKEN_LITERAL_SINTEGER,

	TOKEN_KEYWORD_IF,

	TOKEN_EOF,

	TOKEN_COUNT
};

struct Token {
	TokenType type = TOKEN_EOF;
	Location loc;
	String value;
};

static const char symbol_table[] = {
	'=',
	'>',
	'<',
	'{',
	'}',
	'(',
	')',
	'!'
};

struct Tokenizer {
	String full_path;
	String source;
	String buffer;

	u64 prev_line = 0;
	u64 prev_column = 0;

	u64 line = 0;
	u64 column = 0;

	usize index = 0;
	u8 current_8bits = '\0';
};

struct Parser {
};

Tokenizer create_tokenizer(const char *full_path, const char *source);
void destroy_tokenizer(Tokenizer *t);

void skip_whitespace(Tokenizer *t);
Token next_token(Tokenizer *t);
