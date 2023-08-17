#ifndef TEXT_H
#define TEXT_H
#include "texture.h"
#include "containers/dynstring.h"
#include "containers/stretchy_buffer.h"
void drawStringHud(string_t str, RenderData_t* data, float x, float y, float size);
#endif