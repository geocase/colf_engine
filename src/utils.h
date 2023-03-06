#ifndef PLATFORMER_UTILS_H
#define PLATFORMER_UTILS_H
typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGBColor_t;

float normalize(float min, float max, float value);
#endif // PLATFORMER_UTILS_H
