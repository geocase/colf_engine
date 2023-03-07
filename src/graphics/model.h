#ifndef PLATFORMER_MODEL_H
#define PLATFORMER_MODEL_H

#include <cglm/cglm.h>

typedef struct {
	float* vertices;
	float* texture_coordinates;
	size_t tris;
} TexturedModel_t;

const TexturedModel_t unit_cube;

const float unit_cube_vertices[108];
const size_t unit_cube_vertices_size;
const float face_texture_coordinates[108];

const size_t unit_cube_face_size;
const float unit_cube_south_vertices[3 * 6];

#endif // PLATFORMER_MODEL_H
