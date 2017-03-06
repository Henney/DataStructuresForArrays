#include "stdafx.h"
#include "SimplePerfTimer.h"
#include "SimplePerfTimer.cpp"
#include "TieredVector.cpp"
#include <vector>
#include <random>
// TODO: Stuff is corrupted when using .h file...

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
		cd.insertElemAt(10, n+i);
	}

	for (int i = 10; i >= 0; i--) {
		cd.removeElemAt(n);
	}

	for (int i = 0; i < n; i++) {
		cd.insertElemAt(n, n*2 + i);
	}

	cout << cd.toString() << endl << cd.toStringPretty() << endl;

	//delete &cd;
}


void testK2() {
	TieredVector tv = TieredVector(2);

	int n = 16;

	for (int i = 0; i < n; i++) {
		tv.insertElemAt(i, i);
	}
	/*
	for (int i = 4; i >= 0; i--) {
		tv.removeElemAt(i);
	}*/

	for (int i = 0; i < 10; i++) {
		tv.insertElemAt(5, n + i);
	}

	/*for (int i = 0; i < n; i++) {
		tv.insertElemAt(n+10, n*2 + i);
	}*/

}

void benchmarkInsertion()
{
	int n = 1;

	SIMPLEPERF_FUNCSTART;

	for (int i = 1; i <= 15; i++) {
		n = n*2;
		cout << "Size: " << n << endl;

		TieredVector tv(2);
		SIMPLEPERF_START("Inserting into TieredVector");
		for (int i = 0; i < n; i++) {
			int no = std::rand() % (tv.n + 1);
			//cout << endl << "i: " << i << " r: " << no << endl;
			tv.insertElemAt(no, i);
		}
		SIMPLEPERF_END;

		/*
		SIMPLEPERF_START("Removing from TieredVector");
		for (int i = 0; i < n; i++) {
			tv.removeElemAt(0);
		}
		SIMPLEPERF_END;
		*/

		CircularDeque cd = CircularDeque(true);

		SIMPLEPERF_START("Inserting into CircularDeque");
		for (int i = 0; i < n; i++) {
			int no = rand() % (cd.n + 1);
			cd.insertElemAt(no, i);
		}
		SIMPLEPERF_END;
		/*
		SIMPLEPERF_START("Removing from CircularDeque");
		for (int i = 0; i < n; i++) {
			cd.removeElemAt(0);
		}
		SIMPLEPERF_END;
		*/
		std::vector<int> v;

		SIMPLEPERF_START("Inserting into vector");
		for (int i = 0; i < n; i++) {
			int no = rand() % (v.size() + 1);
			v.insert(v.begin() + no, i);
		}
		SIMPLEPERF_END;
		/*
		SIMPLEPERF_START("Removing from vector");
		for (int i = 0; i < n; i++) {
			v.pop_back();
		}
		SIMPLEPERF_END;
		*/
	}
	SIMPLEPERF_REPORTALL;

}

int main()
{
	//testK1();
	testK2();

	//benchmarkInsertion();

	string s;
	cin >> s;

	return 0;
}