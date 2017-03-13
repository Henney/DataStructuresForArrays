#include "stdafx.h"
#include "K2TieredVector.cpp"
#include <set>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

#define MIN(a, b) (a < b ? a : b)
#define SEED 3938
#define TIMER_FUNC get_wall_time

#define TESTS 1000
#define INSERTIONS 1000
#define INTERVAL 1000
#define AMOUNT 4

double* benchmarkInsertionArray() {
	int maxElems = MIN(200000, INTERVAL*TESTS); // Takes too long otherwise
	int tests = maxElems / INTERVAL;
	double* result = (double*) calloc(TESTS, sizeof 0.0);

	int l = 1;
	int *a = new int[l];
	for (int i = 1; i <= tests; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < INSERTIONS; j++) {
			int no = rand() % (l + 1);
			a = arrayInsert(a, l, no, j);
			l++;
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - INSERTIONS; j++) {
			int no = rand() % (l + 1);
			a = arrayInsert(a, l, l, j);
			l++;
		}
	}
	return result;
}

double* benchmarkInsertionVector() {
	vector<int> v;
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	for (int i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < INSERTIONS; j++) {
			int no = rand() % (v.size() + 1);
			v.insert(v.begin() + no, j);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - INSERTIONS; j++) {
			v.push_back(j);
		}
	}
	return result;
}


double* benchmarkInsertionBST() {
	set<int> s;
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	for (int i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		unsigned int elems = s.size();
		for (int j = elems; j < elems + INSERTIONS; j++) {
			s.insert(j);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		elems = s.size();
		for (int j = elems; j < elems + INTERVAL - INSERTIONS; j++) {
			s.insert(j);
		}
	}
	return result;
}

double* benchmarkInsertionK2TieredVector() {
	K2TieredVector tv(2);
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	for (int i = 1; i <= TESTS; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < INSERTIONS; j++) {
			int no = rand() % (tv.n + 1);
			tv.insertElemAt(no, j);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - INSERTIONS; j++) {
			tv.insertLast(j);
		}
	}
	return result;
}

double* benchmarkInsertionBitTrickK2TieredVector() {
	K2TieredVector tv(2);
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	for (int i = 1; i <= TESTS; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < INSERTIONS; j++) {
			int no = rand() % (tv.n + 1);
			tv.insertElemAt(no, j);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - INSERTIONS; j++) {
			tv.insertLast(j);
		}
	}
	return result;
}

void benchmarkInsertion()
{

	// Data structures: array, vector, (bst = red-black tree i.e. bbst), tiered vector
	double* (*functions[AMOUNT])() = {
		&benchmarkInsertionArray
		,
		&benchmarkInsertionVector
		,
		&benchmarkInsertionBST
		,
		&benchmarkInsertionK2TieredVector
		//, 
		//&benchmarkInsertionBitTrickK2TieredVector
	};
	
	double results[AMOUNT][TESTS];
	for (int i = 0; i < AMOUNT; i++) {
		double* result = functions[i]();
		for (int j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
		cout << "One done" << endl;
	}

	ofstream outfile;
	outfile.open("insertion.txt");
	outfile << "size;array;vector;bst;tiered vector\n";
	for (int i = 0; i < TESTS; i++) {
		outfile << INTERVAL * i;
		for (int j = 0; j < AMOUNT; j++) {
			outfile << ";" << results[j][i];
		}
		outfile << "\n";
	}
	outfile.close();
}
/*
void benchmarkRemoval()
{
	int n = 1;
	int x = 100;
	int TESTS = 10;
	int INTERVAL = 100000;

	SIMPLEPERF_FUNCSTART;
	cout << "Vector" << endl;
	srand(1337);
	vector<int> v;

	for (int i = 0; i < x*INTERVAL; i++) {
		v.push_back(i);
	}

	for (int i = 1; i <= x; i++) {
		int size = v.size();
		cout << "[" << size << ",";

		SIMPLEPERF_START();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % (v.size() + 1);
			v.erase(v.begin() + no);
		}
		SIMPLEPERF_END;

		for (int j = 0; j < INTERVAL - TESTS; j++) {
			v.pop_back();
		}

		cout << "], " << endl;
	}

	cout << "Tiered vector" << endl;
	srand(1337);
	K2TieredVector tv(2);

	for (int i = 0; i < x*INTERVAL; i++) {
		tv.insertElemAt(i, i);
	}

	for (int i = 1; i <= x; i++) {
		int size = tv.n;
		cout << "[" << size << ",";

		SIMPLEPERF_START();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % tv.n;
			tv.removeElemAt(no);
		}
		SIMPLEPERF_END;

		for (int j = 0; j < INTERVAL - TESTS; j++) {
			tv.removeLast();
		}

		cout << "], " << endl;
	}
	SIMPLEPERF_REPORTALL;

}

void benchmarkAccess()
{
	int n = 1;
	int x = 100;
	int INTERVAL = 100;
	int TESTS = 1000;
	const int l = 10000; // Must be x * INTERVAL

	SIMPLEPERF_FUNCSTART;
	cout << "Array" << endl;
	for (int i = 1; i <= x; i++) {
		int size = i * INTERVAL;
		cout << "[" << size << ",";

		srand(1337);
		int a[10000];

		for (int j = 0; j < size; j++) {
			a[j] = j;
		}

		SIMPLEPERF_START();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % l;
			a[no];
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}

	cout << "Vector" << endl;
	for (int i = 1; i <= x; i++) {
		int size = i * INTERVAL;
		cout << "[" << size << ",";

		srand(1337);
		vector<int> v;

		for (int j = 0; j < size; j++) {
			v.insert(v.begin() + j, j);
		}

		SIMPLEPERF_START();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % v.size();
			v.at(no);
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}

	cout << "Tiered vector" << endl;
	for (int i = 1; i <= x; i++) {
		int size = i * INTERVAL;
		cout << "[" << size << ",";

		srand(1337);
		K2TieredVector tv(2);

		for (int j = 0; j < size; j++) {
			tv.insertElemAt(j, j);
		}

		SIMPLEPERF_START();
		for (int j = 0; j < TESTS; j++) {
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
	int INTERVAL = 50;
	int TESTS = 100;

	SIMPLEPERF_FUNCSTART;
	cout << "Tiered vector" << endl;

	for (int n = INTERVAL; n < preInsert; n += INTERVAL) {
		cout << "[" << n << ",";

		K2TieredVector tv(2, n);

		for (int i = 0; i < preInsert; i++) {
			tv.insertElemAt(0, i);
		}

		SIMPLEPERF_START();
		for (int i = 0; i < TESTS; i++) {
			tv.insertElemAt(0, i);
		}
		SIMPLEPERF_END;

		cout << "], " << endl;
	}
	SIMPLEPERF_REPORTALL;

}*/

int main()
{
	benchmarkInsertion();
	//benchmarkRemoval();
	//benchmarkAccess();
	//benchmarkSize();

	string s;
	cin >> s;
	return 0;
}