#ifndef PLATFORMER_LEVEL_H
#define PLATFORMER_LEVEL_H
#include <stdbool.h>

#include "containers/stretchy_buffer.h"
#include "graphics/model.h"
#include "utils.h"

#define MAP_SIZE 16

typedef struct {
	bool solid;
	RGBColor_t color;
	int tex_index[6];
} Tile_t;

typedef struct {
	Tile_t data[MAP_SIZE * MAP_SIZE];
} Map_t;

TexturedModel_t generateLevelGeometry(Map_t *map);

#endif // PLATFORMER_LEVEL_H
