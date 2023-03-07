#ifndef COLF_LEVEL_H
#define COLF_LEVEL_H
#include <stdbool.h>

#include "containers/stretchy_buffer.h"
#include "graphics/model.h"
#include "utils.h"
#include "entity.h"

#define MAP_SIZE 16

typedef struct {
	bool solid;
	RGBColor_t color;
	int tex_index[6];
} Tile_t;

typedef struct {
	Entity_t entity_placements;
	Tile_t data[MAP_SIZE * MAP_SIZE];
} Map_t;

TexturedModel_t generateLevelGeometry(Map_t *map);

#endif // COLF_LEVEL_H
