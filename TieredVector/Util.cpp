#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <new>
#include <array>
#include <string>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

class CircularDeque;
class TieredVector;

static void checkIndexOutOfBounds(int r, int size, string op, string type) {
	if (r >= size || r < 0) {
		cerr << "Tried to " << op << " element at rank " << r << " on array of size " << size << " in " << type << endl;
		string s;
		cin >> s;
		exit(-1);
	}
}