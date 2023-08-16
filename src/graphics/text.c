#include <SDL2_ttf/SDL_ttf.h>
#include "text.h"

void newStringTexture(string_t str, StringTexture_t* str_tex) {
    // destroyString(&str_tex->rendered_string);
    str_tex->rendered_string = str;

    TTF_Font* pt_serif = TTF_OpenFont("run_data/PTSerif-Regular.ttf", 14);
	SDL_Surface* hello_surf = TTF_RenderText_Blended(pt_serif, str_tex->rendered_string.str, (SDL_Color){255, 0, 0, 255});
	if(hello_surf == NULL) {
		printf("%s\n", TTF_GetError());
	}
	loadTextureFromSDLSurface(hello_surf, &str_tex->texture);
}