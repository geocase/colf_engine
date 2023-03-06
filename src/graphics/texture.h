#ifndef PLATFORMER_TEXTURE_H
#define PLATFORMER_TEXTURE_H
typedef struct {
	int w, h;
	char *data; // format is normalized;
} TextureData_t;

typedef struct {
	int w, h;
	unsigned int index;
} glTexture_t;
void loadImage(const char *path, TextureData_t *out);
glTexture_t pushTextureToGPU(TextureData_t* text);
#endif // PLATFORMER_TEXTURE_H
