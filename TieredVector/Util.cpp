#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <new>
#include <array>
#include <string>

using namespace std;

// From http://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows
// Only works in Windows
#include <Windows.h>

double get_wall_time() {
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) {
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)) {
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}

double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |
			((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else {
		//  Handle error
		return 0;
	}
}

// TODO: Add arrayRemove
int* arrayInsert(int *a, int l, int r, int e) {
	int *newA = new int[l + 1];
	for (int k = 0; k < r; k++) {
		newA[k] = a[k];
	}
	for (int k = r; k < l; k++) {
		newA[k + 1] = a[k];
	}
	delete[] a;
	newA[r] = e;
	return newA;
}

class CircularDeque;
class K2TieredVector;
class BitTrickK2TieredVector;

static void checkIndexOutOfBounds(int r, int size, string op, string type) {
	if (r >= size || r < 0) {
		cerr << "Tried to " << op << " element at rank " << r << " on array of size " << size << " in " << type << endl;
		string s;
		cin >> s;
		exit(-1);
	}
}