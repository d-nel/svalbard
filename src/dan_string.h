#include "types.h"

struct String {
	usize size = 0; // strlen aka not including null-terminator
	usize capacity = 0;
	char *data = NULL;
};

String create_string(const char *cstr);
void destroy_string(String *str);

void append(String *str, char c);
