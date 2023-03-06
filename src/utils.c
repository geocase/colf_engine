#include "utils.h"

float normalize(float min, float max, float value) {
	return (max - value) / (max - min);
}
