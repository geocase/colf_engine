#ifndef COLF_UTILS_H
#define COLF_UTILS_H
#define SGN(__X) ((__X > 0) - (__X < 0))

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGBColor_t;

float normalize(float min, float max, float value);
#endif // COLF_UTILS_H
