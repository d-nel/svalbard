#pragma once

#include "string.h"

#include "types.h"

struct String {
	usize size = 0; // strlen aka not including null-terminator
	usize capacity = 0;
	char *data = NULL;

};
inline bool operator== (const String& lhs, const char *rhs) {
	return strcmp(lhs.data, rhs) == 0;
};

inline bool operator== (const String& lhs, const String& rhs) {
	if (lhs.size != rhs.size) return false;
	return strcmp(lhs.data, rhs.data) == 0;
};

String create_string(const char *cstr);
void destroy_string(String *str);

void append(String *str, char c);
