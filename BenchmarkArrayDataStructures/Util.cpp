#include "stdafx.h"
#include <stdint.h>

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

int32_t* arrayInsert(int32_t *a, int32_t l, int32_t r, int32_t e) {
	int32_t *newA = new int32_t[l + 1];
	for (int32_t k = 0; k < r; k++) {
		newA[k] = a[k];
	}
	for (int32_t k = r; k < l; k++) {
		newA[k + 1] = a[k];
	}
	delete[] a;
	newA[r] = e;
	return newA;
}

int32_t* arrayRemove(int32_t *a, int32_t l, int32_t r) {
	int32_t *newA = new int32_t[l - 1];
	for (int32_t k = 0; k < r; k++) {
		newA[k] = a[k];
	}
	for (int32_t k = r; k < l - 1; k++) {
		newA[k] = a[k + 1];
	}
	delete[] a;
	return newA;
}