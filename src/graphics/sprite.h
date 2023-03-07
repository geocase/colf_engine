#ifndef COLF_SPRITE_H
#define COLF_SPRITE_H
#include "renderer.h"
#include "texture.h"

void billboardSpriteInit();
void drawSpriteBillboard(glTexture_t texture, RenderData_t *render_data, float x, float y, float z);
#endif // COLF_SPRITE_H
