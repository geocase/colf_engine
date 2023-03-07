#include <math.h>
#include <stdio.h>
#include "entity_functions.h"

bool entityCanMove(Entity_t* entity, Map_t* map, float angle, float distance) {
	float x_delta = cosf(angle);
	float y_delta = sinf(angle);
	float temp_distance = min(distance / 2.0f, 0.5f);
	while(temp_distance <= distance) {
		vec2 new_position = {entity->position[0] + x_delta * temp_distance + x_delta * entity->radius,
							 entity->position[1] + y_delta * temp_distance + y_delta * entity->radius};
		int rounded_x = (int)new_position[0];
		int rounded_y = (int)new_position[1];
		if(map->data[MAP_SIZE * rounded_y + rounded_x].solid) {
			return false;
		}

		temp_distance += .4;
	}

	return true;
}

void entityWalkTowardsPoint(Entity_t* entity, Map_t* map, float speed, vec2 position) {
	float angle = atan2f(position[1] - entity->position[1], position[0] - entity->position[0]);
	entityWalk(entity, map, speed, angle);
	return;
}

void entityWalk(Entity_t* entity, Map_t* map, float speed, float angle) {
	if(entityCanMove(entity, map, angle, speed)) {
		entity->position[0] += cosf(angle) * speed;
		entity->position[1] += sinf(angle) * speed;
	}
	return;
}

void entityWalkForward(Entity_t* entity, Map_t* map, float speed) {
	entityWalk(entity, map, speed, entity->angle);
	return;
}