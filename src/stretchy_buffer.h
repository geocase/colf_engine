//
// Created by csg on 3/5/2023.
//

#ifndef PLATFORMER_STRETCHY_BUFFER_H
#define PLATFORMER_STRETCHY_BUFFER_H

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
void stretchyBufferClear(StretchyBuffer_t* sb);

#endif // PLATFORMER_STRETCHY_BUFFER_H
