#include "text.h"
#include "graphics/renderer.h"
#include "graphics/sprite.h"
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

TTF_Font *courier = NULL; // temp
// simple cache, should be expanded, possibly integrated into the resource system.
glTexture_t glyphs[UINT8_MAX] = {0};
bool glyph_init[UINT8_MAX] = {0};
void drawStringHud(string_t str, RenderData_t *data, float x, float y, float size) {
	if(courier == NULL) {
		courier = TTF_OpenFont("run_data/CourierPrime-regular.ttf", size);
	}
	glTexture_t glyph_real;
	TextureData_t glyph_text;
	float offset = 0;
	for(size_t i = 0; i < str.length; ++i) {
		int char_index = str.str[i];
		if(!glyph_init[char_index]) {
			SDL_Surface *glyph_surface =
				TTF_RenderGlyph_Blended(courier, str.str[i], (SDL_Color){0, 0, 0, 255});
			generateTextureFromSDLSurface(glyph_surface, &glyph_text);
			glyphs[char_index] = pushTextureToGPU(&glyph_text);
			deleteTexture(&glyph_text);
			SDL_FreeSurface(glyph_surface);
			glyph_init[char_index] = true;
		}

		drawSpriteHud(glyphs[char_index], data, x + offset, y, glyphs[char_index].w,
					  glyphs[char_index].h);
		offset += glyphs[char_index].w;
	}
}
