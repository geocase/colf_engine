#include "dynstring.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

string_t newString(const char *const str) {
	string_t out;
	out.buffer_size = 0;
	out.length = 0;
	out.str = 0;
	appendCStringToString(str, &out);
	return out;
}

void destroyString(string_t *str) {
	free(str->str);
	str->length = 0;
	str->buffer_size = 0;
}

void trimString(string_t *const str, const size_t length) {
	str->length = length;
	str->str[str->length] = '\0';
}

void appendCStringToString(char *cstr, string_t *str) {
	size_t length = strlen(cstr);
	growStringBuffer(str, length + str->length);
	memcpy(str->str + str->length, cstr, (length + 1) * sizeof(char));
	str->length += length; // since we replace the null terminator with the beginning of the string;
	str->str[str->length] = '\0';
	return;
}

void resizeStringBuffer(string_t *const str, size_t size) {
	str->buffer_size = size;
	str->str = realloc(str->str, sizeof(char) * str->buffer_size);
}

void growStringBuffer(string_t *const str, const size_t size) {
	if (str->buffer_size >= size) {
		return;
	} else {
		resizeStringBuffer(str, size);
	}
	return;
}

void shrinkStringBuffer(string_t *const str) {
	resizeStringBuffer(str, str->length + 1); // for null terminator
}

void clearString(string_t *const str) {
	memset(str->str, '\0', str->buffer_size * sizeof(char));
	str->length = 0;
}

void setLength(string_t *const str, size_t length) {
	growStringBuffer(str, length + 1);
	str->length = length;
	str->str[length] = '\0';
}
