#pragma once

#include <iostream>
#include <new>
#include <array>
#include <string>
#include <assert.h>

#define DEFAULT_SIZE 4
#define DEBUG false

using namespace std;

static void checkIndexOutOfBounds(int r, int size, string op, string type);

static void checkIndexOutOfBounds(int r, int size, string op, string type) {
	if (r >= size || r < 0) {
		cerr << "Tried to " << op << " element at rank " << r << " on array of size " << size << " in " << type << endl;
		string s;
		cin >> s;
		exit(-1);
	}
}

double get_cpu_time();
double get_wall_time();
int* arrayInsert(int *a, int l, int r, int e);
int* arrayRemove(int *a, int l, int r);

