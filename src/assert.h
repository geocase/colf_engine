//
// Created by csg on 4/23/2023.
//

#ifndef COLF_ASSERT_H
#define COLF_ASSERT_H
#include <stdio.h>
#include <stdbool.h>
#include "containers/dynstring.h"
void assert(bool cond, string_t message);

#endif // COLF_ASSERT_H
