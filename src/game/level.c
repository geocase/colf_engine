#include <string.h>
#include <stdio.h>

#include "containers/stretchy_buffer.h"
#include "graphics/model.h"
#include "level.h"

TexturedModel_t generateLevelGeometry(Map_t *map) {
	StretchyBuffer_t world_vertices = stretchyBufferInit(sizeof(float));
	StretchyBuffer_t world_texture_coordinates = stretchyBufferInit(sizeof(float));

	// build geometry
	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x) {
			// 4 sides
			// 3 * 6
			bool face_visible[6];
			memset(face_visible, 0, sizeof(bool) * 6);
			if (map->data[MAP_SIZE * y + x].solid) {
				// north face
				if (y - 1 >= 0) {
					if (!map->data[MAP_SIZE * (y - 1) + x].solid) {
						face_visible[0] = true;
					}
				}
				// south face
				if (y + 1 < MAP_SIZE) {
					if (!map->data[MAP_SIZE * (y + 1) + x].solid) {
						face_visible[1] = true;
					}
				}

				// west face
				if (x - 1 >= 0) {
					if (!map->data[MAP_SIZE * (y) + x - 1].solid) {
						face_visible[2] = true;
					}
				}
				// east face
				if (x + 1 < MAP_SIZE) {
					if (!map->data[MAP_SIZE * (y) + x + 1].solid) {
						face_visible[3] = true;
					}
				}
			}

			if (!map->data[MAP_SIZE * (y) + x].solid) {
				face_visible[4] = true;
				face_visible[5] = true;
			}

			for (int face = 0; face < 6; ++face) { // face
				if (!face_visible[face]) {
					continue;
				}
				for (int index = 0; index < 6; ++index) {     // inner face
					for (int coord = 0; coord < 3; ++coord) { // coord
						float k = unit_cube_vertices[(face * 18) + (index * 3) + coord];
						float tex = face_texture_coordinates[(face * 18) + (index * 3) + coord];
						switch (coord) {
						case 0:
							k += x;
							break;
						case 1:
							if (!map->data[MAP_SIZE * y + x].solid) {
								k -= 1;
							}
							if (face == 4) {
								k += 2;
							}
							break;
						case 2:
							k += y;
							tex += map->data[MAP_SIZE * y + x].tex_index[face];
							break;
						}
						stretchyBufferPush(&(k), &world_vertices);
						stretchyBufferPush(&(tex), &world_texture_coordinates);
					}
				}
			}
		}
	}

	TexturedModel_t model;
	model.tris = world_vertices.length / 3;
	model.vertices = malloc(sizeof(float) * model.tris * 3);
	model.texture_coordinates = malloc(sizeof(float) * model.tris * 3);

	memcpy(model.vertices, world_vertices.buffer, sizeof(float) * model.tris * 3);
	memcpy(model.texture_coordinates, world_texture_coordinates.buffer, sizeof(float) * model.tris * 3);

	stretchyBufferFree(&world_vertices);
	stretchyBufferFree(&world_texture_coordinates);

	return model;
}