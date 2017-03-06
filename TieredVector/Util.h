#pragma once

#include <math.h>
#include <iostream>
#include <new>
#include <array>
#include <string>

//#define TieredVector_EXPORTS TRUE
//#ifdef TieredVector_EXPORTS
//#define TV_API __declspec(dllexport)
#define DEFAULT_SIZE 4
#define DEBUG true

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

static void checkIndexOutOfBounds(int r, int size, string op, string type);