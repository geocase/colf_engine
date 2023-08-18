//
// Created by csg on 3/5/2023.
//

#ifndef COLF_STRETCHY_BUFFER_H
#define COLF_STRETCHY_BUFFER_H

#include <stddef.h>
typedef struct {
	size_t length;
	size_t size;
	size_t type_size;
	size_t index_pointer;
	void *buffer;
} StretchyBuffer_t;

StretchyBuffer_t stretchyBufferInit(size_t size_of_type);
void stretchyBufferPush(void *data, StretchyBuffer_t *sb);
void *stretchyBufferAt(size_t index, StretchyBuffer_t *sb);
void stretchyBufferClear(StretchyBuffer_t *sb);
void stretchyBufferFree(StretchyBuffer_t *sb);
#endif // COLF_STRETCHY_BUFFER_H
