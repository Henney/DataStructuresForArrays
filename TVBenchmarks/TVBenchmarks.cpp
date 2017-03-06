// TVBenchmarks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include "windows.h"
#import "TieredVector.dll"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;


//int main()
//{
//	testK1();
//	testK2();
//	cout << "Test completed" << endl;
//	string s;
//	cin >> s;
//	return 0;
//}

void testK1() {
	CircularDeque cd = CircularDeque(true);

	for (int i = 0; i < 5; i++) {
		cd.insertElemAt(i, i);
		cout << cd.toStringPretty().c_str() << endl;
	}
	for (int i = 0; i < 5; i++) {
		cout << cd.getElemAt(i) << endl;
	}

	for (int i = 4; i >= 0; i--) {
		cd.removeElemAt(i);
		cout << cd.toStringPretty().c_str() << endl;
	}
}


void testK2() {
	TieredVector tv = TieredVector(2);

	for (int i = 0; i < 5; i++) {
		tv.insertElemAt(i, i);
		cout << tv.toStringPretty().c_str() << endl;
	}
	for (int i = 0; i < 5; i++) {
		cout << tv.getElemAt(i) << endl;
	}

	for (int i = 4; i >= 0; i--) {
		tv.removeElemAt(i);
		cout << tv.toStringPretty().c_str() << endl;
	}

	tv.insertElemAt(0, 99);
	cout << tv.toStringPretty().c_str() << endl;
}