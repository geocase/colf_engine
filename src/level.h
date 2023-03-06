#ifndef PLATFORMER_LEVEL_H
#define PLATFORMER_LEVEL_H
#include "utils.h"
#include <stdbool.h>

#define MAP_SIZE 16

typedef struct {
	bool solid;
	RGBColor_t color;
} Tile_t;

typedef struct {
	Tile_t data[MAP_SIZE * MAP_SIZE];
} Map_t;

#endif // PLATFORMER_LEVEL_H
