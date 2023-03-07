#include <math.h>
#include <stdio.h>
#include "entity_functions.h"

bool entityCanMove(Entity_t* entity, Map_t* map, float angle, float distance) {
	float x_delta = cosf(angle);
	float y_delta = sinf(angle);
	vec2 new_position = {entity->position[0] + x_delta * distance + x_delta * entity->radius, entity->position[1] + y_delta * distance + y_delta * entity->radius};
	int rounded_x = (int)new_position[0];
	int rounded_y = (int)new_position[1];

	return !(map->data[MAP_SIZE * rounded_y + rounded_x].solid);
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