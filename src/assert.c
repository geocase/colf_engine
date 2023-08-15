#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
void assert(bool cond, string_t message) {
	if (!cond) {
		printf("ASSERTION_FAILED %s\n", message.str);
		exit(-1);
	}
}