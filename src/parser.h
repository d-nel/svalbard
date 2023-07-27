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
	TOKEN_KEYWORD_ELSE,

	TOKEN_EOF,

	TOKEN_COUNT
};

static const char *Token_Type_Strings[TOKEN_COUNT] = {
	"TOKEN_SYMBOL",
	"TOKEN_IDENTIFIER",

	"TOKEN_LITERAL_BOOL",
	"TOKEN_LITERAL_STRING",
	"TOKEN_LITERAL_FLOAT",
	"TOKEN_LITERAL_UINTEGER",
	"TOKEN_LITERAL_SINTEGER",

	"TOKEN_KEYWORD_IF",
	"TOKEN_KEYWORD_ELSE",

	"TOKEN_EOF",
};

struct Token {
	TokenType type = TOKEN_EOF;
	Location loc;
	String value; // @Todo not all token types need a full string
};

static const char symbol_table[] = {
	'!',
	'@',
	'#',
	'$',
	'%',
	'^',
	'&',
	'*',
	'(',
	')',
	'-',
	'=',
	'+',
	'[',
	'{',
	']',
	'}',
	':',
	';',
	'\\',
	'|',
	',',
	'>',
	//'.',
	'<',
	'/',
	'?'
};

struct Tokenizer {
	String full_path;
	String source;
	String buffer;

	u64 prev_line = 1;
	u64 prev_column = 0;

	u64 line = 1;
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
