#include "disk.h"
#include "assert.h"
#include <stdio.h>
// TODO: disk IO, slurp files, load models, etc.

string_t readTextFile(const char *const path) {
	string_t str = {0};
	FILE *f = fopen(path, "rb");
	assert(f, newString(path)); // better string building, err message
	fseek(f, 0, SEEK_END);
	size_t file_size = ftell(f);
	rewind(f);
	resizeStringBuffer(&str, file_size);
	clearString(&str);
	fread(str.str, sizeof(char), file_size, f);
	setLength(&str, file_size);
	fclose(f);
	return str;
}
