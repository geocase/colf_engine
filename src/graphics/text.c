#include <SDL2_ttf/SDL_ttf.h>
#include "graphics/sprite.h"
#include "text.h"

void drawStringHud(string_t str, RenderData_t* data, float x, float y, float size) {
    TTF_Font* courier = TTF_OpenFont("run_data/CourierPrime-regular.ttf", size);
    glTexture_t glyph_real;
    TextureData_t glyph_text;
    float offset = 0;
    for(size_t i = 0; i < str.length; ++i) {
        SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(courier, str.str[i], (SDL_Color){0, 0, 0, 255});
        generateTextureFromSDLSurface(glyph_surface, &glyph_text);
        glyph_real = pushTextureToGPU(&glyph_text);
        deleteTexture(&glyph_text);
        drawSpriteHud(glyph_real, data, x + offset, y, glyph_text.w, glyph_text.h);
        offset += glyph_text.w;
        deleteGlTexture(glyph_real);
        SDL_FreeSurface(glyph_surface);
    }
    TTF_CloseFont(courier);
}
// void renderStringTexture(string_t str, StringTexture_t* str_tex, ) {
// 	SDL_Surface* surf = TTF_RenderText_Blended(pt_serif, str_tex->rendered_string.str, (SDL_Color){255, 0, 0, 255});
//     loadTextureFromSDLSurface(surf, &(str_tex->texture)
// }