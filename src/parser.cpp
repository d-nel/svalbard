#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <cstdarg>

#include "parser.h"

// @Todo We only support locations that are on a single line, but we should do better than that
void report_location(Tokenizer *t, Location loc) {
	u64 line_count = 1;
	u64 char_count = 0;

	auto code_color  = "\033[0;34m";
	auto error_color = "\033[0;31m";

	printf("    %s", code_color);

	for (u64 i = 0; i < t->source.size; i++) {
		u8 c = t->source.data[i];
		char_count++;

		if (c == '\n') {
			char_count = 0;
			line_count++;
			continue;
		}

		if (line_count == loc.l0) {
			if (char_count == loc.c0) {
				printf("%s", error_color);
			}

			putchar(c);

			if (char_count == loc.c1) {
				printf("%s", code_color);
			}
		}

		if (line_count > loc.l0) {
			break;
		}
	}

	printf("\033[0m\n");
}

void report_error(Tokenizer *t, Location loc, const char *fmt, ...) {

	printf("(%s %llu:%llu) error: ", t->full_path.data, loc.l0, loc.c0);
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");

	report_location(t, loc);
}


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
	if (t->index > t->source.size) {
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

String seize_buffer(Tokenizer *t) {
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

try_again:
	skip_whitespace(t);
	
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
				token.loc.l1 = token.loc.l0;
				token.loc.c1 = token.loc.c0;
				report_error(t, token.loc, "We reached the end of the file before getting a second quote symbol (\") to teminate the string.");
				break;
			}
		}

		advance_by_8bits(t);

		token.loc.l1 = t->prev_line;
		token.loc.c1 = t->prev_column;
		token.value = seize_buffer(t);
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
		token.value = seize_buffer(t);
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

			case '/':
				if (peek_8bits(t) == '/') {
					advance_by_8bits(t);
					advance_by_8bits(t);
					while (t->current_8bits != '\n') {
						advance_by_8bits(t);
					}
					advance_by_8bits(t);
					goto try_again;
				} else {
					store_8bits(t);
					advance_by_8bits(t);
				}
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
				token.loc.l1 = token.loc.l0;
				token.loc.c1 = token.loc.c0;
				report_error(t, token.loc, "Unsupported symbol.");
				advance_by_8bits(t);
				break;
		}

		token.loc.l1 = t->prev_line;
		token.loc.c1 = t->prev_column;
		token.value = seize_buffer(t);

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
		token.value = seize_buffer(t);

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
