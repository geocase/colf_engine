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
void loadImage(const char *path, TextureData_t *out);
glTexture_t pushTextureToGPU(TextureData_t *text);
#endif // COLF_TEXTURE_H
