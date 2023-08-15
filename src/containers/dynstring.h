#ifndef TEST_STRING_H
#define TEST_STRING_H
#include <stddef.h>
typedef struct str {
	char *str;
	size_t length;
	size_t buffer_size;
} string_t;

string_t newString(const char *const str);
void destroyString(string_t *str);
void trimString(string_t *const str, const size_t length);
void appendCStringToString(char *cstr, string_t *str);
void resizeStringBuffer(string_t *const str, size_t size);
void growStringBuffer(string_t *const str, size_t size);
void shrinkStringBuffer(string_t *const str);
void clearString(string_t *const str);
// WARNING: destructive
void setLength(string_t *const str, size_t length);
#endif // TEST_STRING_H
