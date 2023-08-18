#include "entity_functions.h"
#include <math.h>
#include <stdio.h>

#ifdef __APPLE__
#define max(a, b) fmax(a, b)
#define min(a, b) fmin(a, b)
#endif

bool entityCollideAndSlide(Entity_t *entity, Map_t *map, float angle, float distance, vec2 slide) {
	// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_CircleVsRect.cpp
	float x_delta = cosf(angle) * distance;
	float y_delta = sinf(angle) * distance;
	// slow
	vec2 new_position = {entity->position[0] + x_delta, entity->position[1] + y_delta};

	for(int map_y = max((int)new_position[1] - 2, 0);
		map_y < min((int)new_position[1] + 2, MAP_SIZE); ++map_y) {
		for(int map_x = max((int)new_position[0] - 2, 0);
			map_x < min((int)new_position[0] + 2, MAP_SIZE); ++map_x) {
			if(map->data[MAP_SIZE * map_y + map_x].solid) {
				float nx = glm_clamp(new_position[0], map_x, map_x + 1);
				float ny = glm_clamp(new_position[1], map_y, map_y + 1);
				vec2 ray_nearest = {nx - new_position[0], ny - new_position[1]};
				float magnitude =
					sqrtf(ray_nearest[0] * ray_nearest[0] + ray_nearest[1] * ray_nearest[1]);
				float overlap = entity->radius - magnitude;

				if(isnan(overlap) || isinf(overlap))
					overlap = 0;
				if(overlap > 0) {
					glm_vec2_normalize(ray_nearest);
					new_position[0] -= ray_nearest[0] * overlap;
					new_position[1] -= ray_nearest[1] * overlap;
				}
			}
		}
	}

	glm_vec2_sub(new_position, entity->position, slide);
	return true;
}

void entityWalkTowardsPoint(Entity_t *entity, Map_t *map, float speed, vec2 position) {
	float angle = atan2f(position[1] - entity->position[1], position[0] - entity->position[0]);
	entityWalk(entity, map, speed, angle);
	return;
}

void entityMove(Entity_t *entity, Map_t *map, float speed, float angle) {
	entity->position[0] += cosf(angle) * speed;
	entity->position[1] += sinf(angle) * speed;
}

void entityWalk(Entity_t *entity, Map_t *map, float speed, float angle) {
	vec2 slide = {0, 0};
	bool can_walk = entityCollideAndSlide(entity, map, angle, speed, slide);
	entity->position[0] += slide[0];
	entity->position[1] += slide[1];
	return;
}

void entityWalkForward(Entity_t *entity, Map_t *map, float speed) {
	entityWalk(entity, map, speed, entity->angle);
	return;
}