#ifndef TEXT_H
#define TEXT_H
#include "texture.h"
#include "containers/dynstring.h"
typedef struct {
    string_t rendered_string;
    TextureData_t texture;
} StringTexture_t;

void newStringTexture(string_t str, StringTexture_t* str_tex);
void updateStringTexture(string_t str);
#endif