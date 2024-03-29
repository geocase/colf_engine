#ifndef COLF_ENTITY_FUNCTIONS_H
#define COLF_ENTITY_FUNCTIONS_H

#include "entity.h"
#include "level.h"
void entityMove(Entity_t *entity, Map_t *map, float speed, float angle);
bool entityCanMove(Entity_t *entity, Map_t *map, float angle, float distance, vec2 slide);
void entityWalkTowardsPoint(Entity_t *entity, Map_t *map, float speed, vec2 position);
void entityWalk(Entity_t *entity, Map_t *map, float speed, float angle);
void entityWalkForward(Entity_t *entity, Map_t *map, float speed);
#endif // COLF_ENTITY_FUNCTIONS_H
