#pragma once

#include <stdint.h>

double get_cpu_time();
double get_wall_time();
int32_t* arrayInsert(int32_t *a, int32_t l, int32_t r, int32_t e);
int32_t* arrayRemove(int32_t *a, int32_t l, int32_t r);