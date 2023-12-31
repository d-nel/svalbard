#pragma once

#include "types.h"
#include "dan_string.h"
#include "ast.h"

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

static const char *TokenType_Strings[TOKEN_COUNT] = {
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
	Tokenizer t;

	Token current_token;
};

Parser create_parser(const char *full_path);
void destroy_parser(Parser *p);

Ast_Base *next_node(Parser *p);

void print_all_tokens_until_eof(Parser *p);
