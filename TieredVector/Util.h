#pragma once

#include <iostream>
#include <new>
#include <array>
#include <string>

#define DEFAULT_SIZE 4
#define DEBUG false

using namespace std;

static void checkIndexOutOfBounds(int r, int size, string op, string type);
double get_cpu_time();
double get_wall_time();
int* arrayInsert(int *a, int l, int r, int e);

