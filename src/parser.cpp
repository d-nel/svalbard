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


Token tokenize_symbol(Tokenizer *t) {
	auto token = Token{};
	token.type = TOKEN_SYMBOL;
	token.loc.l0 = t->line;
	token.loc.c0 = t->column;

	switch (t->current_8bits) {
		case '=':
		case '>':
		case '<':
			store_8bits(t);
			advance_by_8bits(t);
			if (t->current_8bits == '=') {
				store_8bits(t);
				advance_by_8bits(t);
			}
			break;
	}

	token.loc.l1 = t->prev_line;
	token.loc.c1 = t->prev_column;
	token.value = sieze_buffer(t);
	return token;
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
	if (is_symbol(t->current_8bits)) {
		return tokenize_symbol(t);
	}

	if (is_alpha(t->current_8bits)) {
	}

	return Token{};
}

