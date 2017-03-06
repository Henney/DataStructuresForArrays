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

	int n = 64;

	/*for (int i = 0; i < n; i++) {
		tv.insertElemAt(i, i);
	}
	
	for (int i = 4; i >= 0; i--) {
		tv.removeElemAt(i);
	}

	for (int i = 0; i < 10; i++) {
		tv.insertElemAt(5, n + i);
	}

	for (int i = 0; i < n; i++) {
		tv.insertElemAt(tv.n-10, n*2 + i);
	}*/

	for (int i = 0; i < n; i++) {
		int no = std::rand() % (tv.n + 1);
		tv.insertElemAt(no, i);
	}
}

void benchmarkInsertion()
{
	int n = 1;
	int x = 100;
	int tests = 10;
	int interval = 10000;

	SIMPLEPERF_FUNCSTART;
	cout << "Vector" << endl;
	srand(1337);
	std::vector<int> v;

	for (int i = 1; i <= x; i++) {
		int size = v.size();
		cout << "[" << size << ",";
		
		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % (v.size() + 1);
			v.insert(v.begin() + no, j);
		}
		SIMPLEPERF_END;

		for (int j = 0; j < interval-tests; j++) {
			v.push_back(j);
		}

		cout << "], " << endl;
	}

	cout << "Tiered vector" << endl;
	srand(1337);
	TieredVector tv(2);

	for (int i = 1; i <= x; i++) {
		int size = tv.n;
		cout << "[" << size << ",";
		
		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % (tv.n + 1);
			tv.insertElemAt(no, j);
		}
		SIMPLEPERF_END;

		for (int j = 0; j < interval - tests; j++) {
			tv.insertElemAt(n, j);
		}

		cout << "], " << endl;
	}
	SIMPLEPERF_REPORTALL;

}

void benchmarkRemoval()
{
	int n = 1;
	int x = 100;
	int tests = 10;
	int interval = 10000;

	SIMPLEPERF_FUNCSTART;
	cout << "Vector" << endl;
	srand(1337);
	std::vector<int> v;

	for (int i = 0; i < x*interval; i++) {
		v.push_back(i);
	}

	for (int i = 1; i <= x; i++) {
		int size = v.size();
		cout << "[" << size << ",";

		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % (v.size() + 1);
			v.erase(v.begin() + no);
		}
		SIMPLEPERF_END;

		for (int j = 0; j < interval - tests; j++) {
			v.pop_back();
		}

		cout << "], " << endl;
	}

	cout << "Tiered vector" << endl;
	srand(1337);
	TieredVector tv(2);

	for (int i = 0; i < x*interval; i++) {
		tv.insertElemAt(i, i);
	}

	for (int i = 1; i <= x; i++) {
		int size = tv.n;
		cout << "[" << size << ",";

		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % tv.n;
			tv.removeElemAt(no);
		}
		SIMPLEPERF_END;

		for (int j = 0; j < interval - tests; j++) {
			tv.removeElemAt(n - 1);
		}

		cout << "], " << endl;
	}
	SIMPLEPERF_REPORTALL;

}

void benchmarkAccess()
{
	int n = 1;
	int x = 100;
	int interval = 100;
	int tests = 1000;
	const int l = 10000; // Must be x * interval

	SIMPLEPERF_FUNCSTART;
	cout << "Array" << endl;
	for (int i = 1; i <= x; i++) {
		int size = i * interval;
		cout << "[" << size << ",";

		srand(1337);
		int a[10000];

		for (int j = 0; j < size; j++) {
			a[j] = j;
		}

		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % l;
			a[no];
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}

	cout << "Vector" << endl;
	for (int i = 1; i <= x; i++) {
		int size = i * interval;
		cout << "[" << size << ",";

		srand(1337);
		std::vector<int> v;

		for (int j = 0; j < size; j++) {
			v.insert(v.begin() + j, j);
		}

		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % v.size();
			v.at(no);
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}

	cout << "Tiered vector" << endl;
	for (int i = 1; i <= x; i++) {
		int size = i * interval;
		cout << "[" << size << ",";

		srand(1337);
		TieredVector tv(2);

		for (int j = 0; j < size; j++) {
			tv.insertElemAt(j, j);
		}

		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			int no = rand() % tv.n;
			tv.getElemAt(no);
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}
	SIMPLEPERF_REPORTALL;

}

void benchmarkSize()
{
	int preInsert = 10000;
	int interval = 10;
	int tests = 100;

	SIMPLEPERF_FUNCSTART;
	cout << "Tiered vector" << endl;
	for (int n = 10; n <= preInsert; n += 10) {
		cout << "[" << n << ",";

		TieredVector tv(2, n);

		for (int j = 0; j < preInsert; j++) {
			tv.insertElemAt(j, j);
		}

		SIMPLEPERF_START();
		for (int j = 0; j < tests; j++) {
			tv.insertElemAt(0, j);
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}
	SIMPLEPERF_REPORTALL;

}

int main()
{
	//testK1();
	//testK2();

	benchmarkInsertion();
	//benchmarkRemoval();
	//benchmarkAccess();
	benchmarkSize();

	string s;
	cin >> s;

	return 0;
}