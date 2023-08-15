#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#ifndef __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#ifdef __APPLE__
#include "OpenGL/gl3.h"
#endif

#ifndef __APPLE__
#include <glad/glad.h>
#endif

#include "texture.h"

void flipSurface(SDL_Surface *surf) {
	SDL_LockSurface(surf);
	int pitch = surf->pitch;
	char *temp = malloc(sizeof(char) * pitch);
	char *pixels = (char *)surf->pixels;

	for (int i = 0; i < surf->h / 2; ++i) {
		char *row1 = pixels + i * pitch;
		char *row2 = pixels + (surf->h - i - 1) * pitch;

		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	free(temp);
	SDL_UnlockSurface(surf);
}

void loadImage(const char *path, TextureData_t *out) {
	printf("%s\n", path);
	SDL_Surface *tmp_img = IMG_Load(path);
	SDL_PixelFormat *fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *conv = SDL_ConvertSurface(tmp_img, fmt, 0);
	flipSurface(conv);

	SDL_LockSurface(conv);

	out->w = conv->w;
	out->h = conv->h;
	out->data = malloc(sizeof(char) * out->w * out->h * 4);
	memcpy(out->data, conv->pixels, sizeof(char) * out->w * out->h * 4);

	SDL_UnlockSurface(conv);
	SDL_FreeSurface(conv);
	SDL_FreeSurface(tmp_img);

	return;
}

glTexture_t pushTextureToGPU(TextureData_t *text) {
	unsigned int texture_index;
	glGenTextures(1, &texture_index);
	glBindTexture(GL_TEXTURE_2D, texture_index);
	unsigned int pixel_format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, text->w, text->h, 0, pixel_format, GL_UNSIGNED_BYTE, text->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	return (glTexture_t){text->w, text->h, texture_index};
}
