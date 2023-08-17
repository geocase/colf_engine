#ifndef COLF_TEXTURE_H
#define COLF_TEXTURE_H
typedef struct {
	int w, h;
	char *data; // format is normalized;
} TextureData_t;

typedef struct {
	int w, h;
	unsigned int index;
} glTexture_t;
void generateTextureFromSDLSurface(SDL_Surface* sfc, TextureData_t* out);
void loadImageToTexture(const char *path, TextureData_t *out);
void deleteTexture(TextureData_t* del);
glTexture_t pushTextureToGPU(TextureData_t *text);
void deleteGlTexture(glTexture_t gltex);
#endif // COLF_TEXTURE_H
