#include "stdafx.h"
#include "K2TieredVector.cpp"

void testK1() {
	CircularDeque cd = CircularDeque(true);

	int n = 16;

	for (int i = 0; i < n; i++) {
		cd.insertElemAt(i, i);
	}

	for (int i = 4; i >= 0; i--) {
		cd.removeElemAt(i);
	}

	for (int i = 0; i < n; i++) {
		cd.insertElemAt(10, n + i);
	}

	for (int i = 10; i >= 0; i--) {
		cd.removeElemAt(n);
	}

	for (int i = 0; i < n; i++) {
		cd.insertElemAt(n, n * 2 + i);
	}

	cout << cd.toString() << endl << cd.toStringPretty() << endl;
}

void testK2() {
	K2TieredVector tv = K2TieredVector(2);

	int n = 64;

	for (int i = 0; i < n; i++) {
		//int no = rand() % (tv.n + 1);
		tv.insertElemAt(i, i);
	}

	for (int i = 0; i < n; i++) {
		tv.removeLast();
		cout << tv.toString() << endl << tv.toStringPretty() << endl;
	}
}