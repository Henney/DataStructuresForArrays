#pragma once

#include <iostream>
#include <new>
#include <array>
#include <string>
#include <random>
#include <cassert>

using namespace std;

#define DEFAULT_SIZE 4
#define nBelow0 "n is below 0 in CircularDeque!"

static void checkIndexOutOfBounds(int32_t r, int32_t size, string op, string type);
static void checkIndexOutOfBounds(int32_t r, int32_t size, string op, string type) {
	if (r >= size || r < 0) {
		throw out_of_range(
			"Tried to " + op + " element at rank " + to_string(r) +
			" on array of size " + to_string(size) + " in " + type
		);
	}
}

class ArrayDataStructure {
public:
	virtual uint32_t size() = 0;

	const int32_t operator[](const int32_t index) { return getElemAt(index); }

	virtual int32_t getElemAt(int32_t) = 0;
	virtual void setElemAt(int32_t, int32_t) = 0;
	virtual void insertElemAt(int32_t, int32_t) = 0;
	virtual int32_t removeElemAt(int32_t) = 0;

	virtual void insertLast(int32_t) = 0;
	virtual int32_t removeLast() = 0;

	virtual string toString() = 0;
	virtual string toStringPretty() = 0;
};