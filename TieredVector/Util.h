#pragma once

#include <iostream>
#include <new>
#include <array>
#include <string>
#include <random>

using namespace std;

#define DEFAULT_SIZE 4
#define nBelow0 "n is below 0 in CircularDeque!"

static void checkIndexOutOfBounds(int32_t r, int32_t size, string op, string type);

static void checkIndexOutOfBounds(int32_t r, int32_t size, string op, string type) {
	if (r >= size || r < 0) {
		cerr << "Tried to " << op << " element at rank " << r << " on array of size " << size << " in " << type << endl;
		string s;
		cin >> s;
		exit(-1);
	}
}
static void warnNBelow0(void);

static void warnNBelow0() {
#if DEBUG
	cout << nBelow0 << endl;
	string s;
	cin >> s;
	throw exception(nBelow0);
#endif
}

class RankSequence {
public:
	int32_t n = 0;
	//int operator[](int index)
	//{
	//	return getElemAt(index);
	//}

	virtual int32_t getElemAt(int32_t r) = 0;
	virtual void insertElemAt(int32_t r, int32_t e) = 0;
	virtual int32_t removeElemAt(int32_t r) = 0;

	virtual void insertLast(int32_t e) = 0;
	virtual int32_t removeLast() = 0;
};