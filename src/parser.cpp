#include <string.h>
#include <assert.h>

#include "parser.h"

inline bool is_symbol(u8 c) {
	for (usize i = 0; i < array_sizeof(symbol_table); i++) {
		if (c == symbol_table[i]) return true;
	}

	return false;
}

inline bool is_whitespace(u8 c) {
	return (c == '\r' || c == '\n' || c == ' ' || c == '\t');
}

inline bool is_digit(u8 c) {
	return c >= '0' && c <= '9';
}

inline bool is_alpha(u8 c) {
	return (c >= 'a' && c<= 'z') || (c >= 'A' && c <= 'Z');
}

void advance_by_8bits(Tokenizer *t) {
	if (t->index >= t->source.size) {
		t->current_8bits = '\0';
		return;
	}

	t->current_8bits = t->source.data[t->index];
	t->index++;

	if (t->current_8bits == '\n') {
		t->prev_line = t->line;
		t->line += 1;
		t->prev_column = t->column;
		t->column = 0;
	} else if ((t->current_8bits & 0xC0) != 0x80) {
		// if not a continuation char, then increse column
		t->prev_column = t->column;
		t->column += 1;
	}
}

u8 peek_8bits(Tokenizer *t) {
	if (t->index >= t->source.size) {
		return '\0';
	}

	return t->source.data[t->index];
}

void store_8bits(Tokenizer *t) {
	if (t->current_8bits != '\0') {
		append(&t->buffer, t->current_8bits);
	}
}

String sieze_buffer(Tokenizer *t) {
	auto token_buffer = t->buffer;

	t->buffer = create_string("");

	return token_buffer;
}

Tokenizer create_tokenizer(const char *full_path, const char *source) {
	auto t = Tokenizer{};

	t.full_path = create_string(full_path);
	t.source = create_string(source);
	t.buffer = create_string("");

	advance_by_8bits(&t);

	return t;
}


void destroy_tokenizer(Tokenizer *t) {
	destroy_string(&t->full_path);
	destroy_string(&t->source);
	destroy_string(&t->buffer);

	*t = Tokenizer{}; // @Cleanup is this necessary?
}


void skip_whitespace(Tokenizer *t) {
	while (is_whitespace(t->current_8bits)) {
		advance_by_8bits(t);
	}
}

Token next_token(Tokenizer *t) {
	auto token = Token{};
	token.loc.l0 = t->line;
	token.loc.c0 = t->column;

	//
	// Tokenize String
	//
	if (t->current_8bits == '"') {
		token.type = TOKEN_LITERAL_STRING;

		assert(t->current_8bits == '"');
		advance_by_8bits(t);

		while (t->current_8bits != '"') {
			store_8bits(t);
			advance_by_8bits(t);

			if (t->current_8bits == '\0') {
				// @Todo error
				assert(false);
			}
		}

		advance_by_8bits(t);

		token.loc.l1 = t->prev_line;
		token.loc.c1 = t->prev_column;
		token.value = sieze_buffer(t);
		return token;
	}

	//
	// Tokenize Number Literal
	//
	if ((t->current_8bits == '-' && is_digit(peek_8bits(t))) || is_digit(t->current_8bits)) {
		auto is_negative = t->current_8bits == '-';
		auto is_float = false;

		if (is_negative) {
			store_8bits(t);
			advance_by_8bits(t);
		}

		while(is_digit(t->current_8bits) || (t->current_8bits == '.' && !is_float)) {
			if (t->current_8bits == '.') {
				is_float = true;
			}

			store_8bits(t);
			advance_by_8bits(t);
		}

		if (is_float) {
			token.type = TOKEN_LITERAL_FLOAT;
		} else if (is_negative) {
			token.type = TOKEN_LITERAL_SINTEGER;
		} else {
			token.type = TOKEN_LITERAL_UINTEGER;
		}

		
		token.loc.l1 = t->prev_line;
		token.loc.c1 = t->prev_column;
		token.value = sieze_buffer(t);
		return token;
	}

	//
	// Tokenize Symbol
	//
	if (is_symbol(t->current_8bits)) {
		token.type = TOKEN_SYMBOL;

		switch (t->current_8bits) {
			case '{':
			case '}':
			case '(':
			case ')':
			case '+':
			case '-':
			case '*':
			case ';':
				store_8bits(t);
				advance_by_8bits(t);
				break;

			case '=':
			case '!':
			case '>':
			case '<':
				store_8bits(t);
				advance_by_8bits(t);
				if (t->current_8bits == '=') {
					store_8bits(t);
					advance_by_8bits(t);
				}
				break;

			default:
				// @Todo Report an error for invaild/unimplemented symbol.
				break;
		}

		token.loc.l1 = t->prev_line;
		token.loc.c1 = t->prev_column;
		token.value = sieze_buffer(t);
		return token;
	}

	//
	// Tokenize Keyword, Boolean Literal, or Identifier
	//
	if (is_alpha(t->current_8bits)) {
		token.type = TOKEN_IDENTIFIER;

		bool can_be_keyword_or_bool = true;

		while (is_alpha(t->current_8bits) || t->current_8bits == '_' || is_digit(t->current_8bits)) {
			if (!is_alpha(t->current_8bits)) can_be_keyword_or_bool = false;

			store_8bits(t);
			advance_by_8bits(t);
		}

		token.loc.l1 = t->prev_line;
		token.loc.c1 = t->prev_column;
		token.value = sieze_buffer(t);

		if (can_be_keyword_or_bool) {
			if (strcmp(token.value.data, "if") == 0) {
				token.type = TOKEN_KEYWORD_IF;
			} else if (strcmp(token.value.data, "true") == 0) {
				token.type = TOKEN_LITERAL_BOOL;
			} else if (strcmp(token.value.data, "false") == 0) {
				token.type = TOKEN_LITERAL_BOOL;
			}

		}

		return token;
	}

	return Token{};
}
