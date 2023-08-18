#include <stdlib.h>
#include <string.h>

#include "stretchy_buffer.h"

StretchyBuffer_t stretchyBufferInit(size_t size_of_type) {
	StretchyBuffer_t out;
	out.length = 0;
	out.size = 64; // initial size of 4;
	out.type_size = size_of_type;
	out.index_pointer = 0;
	out.buffer = malloc(out.type_size * out.size);
	return out;
}

void stretchyBufferPush(void *data, StretchyBuffer_t *sb) {
	memcpy((char *)sb->buffer + (sb->index_pointer * sb->type_size), data, sb->type_size);
	sb->index_pointer += 1; // adjust everything after;
	sb->length += 1;
	if(sb->length == sb->size) {
		sb->size *= 2;
		sb->buffer = realloc(sb->buffer, sb->type_size * sb->size);
	}
	return;
}

void *stretchyBufferAt(size_t index, StretchyBuffer_t *sb) {
	return (void *)((char *)sb->buffer + ((index % sb->length) * sb->type_size));
}

void stretchyBufferClear(StretchyBuffer_t *sb) {
	memset(sb->buffer, 0, sb->type_size * sb->size);
	sb->length = 0;
	sb->index_pointer = 0;
	return;
}

void stretchyBufferFree(StretchyBuffer_t *sb) {
	free(sb->buffer);
	memset(sb, 0, sizeof(StretchyBuffer_t));
	return;
}
