#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dan_string.h"

String create_string(const char *cstr) {
	auto str = String{};

	str.size = strlen(cstr);
	str.capacity = str.size + 1;

	str.data = (char *)malloc(sizeof(char) * (str.capacity));

	memcpy((void*)str.data, (void*)cstr, str.size);
	str.data[str.size] = '\0';

	return str;
}

void destroy_string(String *str) {
	if (!str->data) return;

	free((void*)str->data);
	str->capacity = 0;
	str->size = 0;
	str->data = NULL;
}


void append(String *str, char c) {
	assert(str->data);

	if (str->size + 1 == str->capacity) {
		str->capacity *= 2;

		void *new_data = malloc(sizeof(char) * (str->capacity));

		memmove(new_data, (void*)str->data, str->size + 1);
		str->data = (char *)new_data;
	}

	str->data[str->size] = c;
	str->size++;
	str->data[str->size] = '\0';
}
