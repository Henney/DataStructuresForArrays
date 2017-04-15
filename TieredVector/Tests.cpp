#include "stdafx.h"
#include "TieredVector.cpp"
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
#define DELETIONS 1000
#define INTERVAL 1000
#define AMOUNT 5

#define ARRAY_AMOUNT 200000

/*
	Insertion
*/

double* benchmarkInsertionArray() {
	int maxElems = MIN(ARRAY_AMOUNT, INTERVAL*TESTS); // Takes too long otherwise
	int tests = maxElems / INTERVAL;
	double* result = new double[TESTS];

	int l = 1;
	int *a = new int[l];
	for (int i = 0; i < tests; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < INSERTIONS; j++) {
			int no = rand() % (l + 1);
			a = arrayInsert(a, l, no, j);
			l++;
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - INSERTIONS; j++) {
			a = arrayInsert(a, l, l, j);
			l++;
		}
	}
	delete[] a;
	return result;
}

double* benchmarkInsertionVector() {
	vector<int> v;
	double* result = new double[TESTS];

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
	double* result = new double[TESTS];

	int total = TESTS * INTERVAL;
	int* numbers = (int*)calloc(total, sizeof 0);
	for (int i = 0; i < total; i++) {
		numbers[i] = i;
	}
	random_shuffle(numbers, numbers + total); // Shuffle the numbers

	for (int i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		unsigned int elems = s.size();
		for (int j = elems; j < elems + INSERTIONS; j++) {
			s.insert(numbers[j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		elems = s.size();
		for (int j = elems; j < elems + INTERVAL - INSERTIONS; j++) {
			s.insert(numbers[j]);
		}
	}
	return result;
}

double* benchmarkInsertionK2TieredVector() {
	K2TieredVector tv;
	double* result = new double[TESTS];

	for (int i = 0; i < TESTS; i++) {
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
	BitTrickK2TieredVector tv;
	double* result = new double[TESTS];	

	for (int i = 0; i < TESTS; i++) {
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

double* benchmarkInsertion3TieredVector() {
	TieredVector tv(3);
	double* result = new double[TESTS];

	for (int i = 0; i < TESTS; i++) {
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
	double* (*functions[6])() = {
		&benchmarkInsertionArray
		,
		&benchmarkInsertionVector
		,
		&benchmarkInsertionBST
		,
		&benchmarkInsertionK2TieredVector
		,
		&benchmarkInsertionBitTrickK2TieredVector
		,
		&benchmarkInsertion3TieredVector
	};
	
	double results[AMOUNT][TESTS];
	for (int i = 0; i < AMOUNT; i++) {
		srand(SEED);
		double* result = functions[i]();
		cout << i << " done" << endl;
		for (int j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
	}

	ofstream outfile;
	outfile.open("insertion.txt");
	outfile << "size;array;vector;bst;tiered vector;bittrick tiered vector\n";
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
	Removal
*/

double* benchmarkRemovalArray() {
	int maxElems = MIN(ARRAY_AMOUNT, INTERVAL*TESTS); // Takes too long otherwise
	int tests = maxElems / INTERVAL;
	double* result = new double[TESTS];

	int l = INTERVAL * tests;
	int *a = new int[l];

	for (int i = 0; i < l; i++) {
		a[i] = i;
	}

	for (int i = 0; i < tests; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < DELETIONS; j++) {
			int no = rand() % l;
			a = arrayRemove(a, l, no);
			l--;
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - DELETIONS; j++) {
			a = arrayRemove(a, l, l - 1);
			l--;
		}
	}
	delete[] a;
	return result;
}

double* benchmarkRemovalVector() {
	vector<int> v;
	double* result = new double[TESTS];

	for (int i = 0; i < INTERVAL * TESTS; i++) {
		v.push_back(i);
	}

	for (int i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < DELETIONS; j++) {
			int no = rand() % v.size();
			v.erase(v.begin() + no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - DELETIONS; j++) {
			v.pop_back();
		}
	}
	assert(v.size() == 0);
	return result;
}


double* benchmarkRemovalBST() {
	set<int> s;
	double* result = new double[TESTS];
	int total = TESTS * INTERVAL;

	for (int i = 0; i < total; i++) {
		s.insert(i);
	}

	int* numbers = (int*)calloc(total, sizeof 0);
	for (int i = 0; i < total; i++) {
		numbers[i] = i;
	}
	random_shuffle(numbers, numbers + total); // Shuffle the numbers

	for (int i = 0; i < TESTS; i++) {
		unsigned int elems = s.size();
		double start = TIMER_FUNC();
		for (int j = 0; j < DELETIONS; j++) {
			s.erase(numbers[elems - j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		elems = s.size();
		for (int j = 0; j < INTERVAL - DELETIONS; j++) {
			s.erase(numbers[elems - j]);
		}
	}
	return result;
}

double* benchmarkRemovalK2TieredVector() {
	double* result = new double[TESTS];

	K2TieredVector tv;

	for (int i = 0; i < TESTS * INTERVAL; i++) {
		tv.insertLast(i);
	}

	for (int i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < DELETIONS; j++) {
			int no = rand() % tv.n;
			tv.removeElemAt(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - DELETIONS; j++) {
			tv.removeLast();
		}
	}

	return result;
}

double* benchmarkRemovalBitTrickK2TieredVector() {
	double* result = new double[TESTS];
	BitTrickK2TieredVector tv;

	for (int i = 0; i < TESTS * INTERVAL; i++) {
		tv.insertLast(i);
	}

	for (int i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int j = 0; j < DELETIONS; j++) {
			int no = rand() % tv.n;
			tv.removeElemAt(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int j = 0; j < INTERVAL - DELETIONS; j++) {
			tv.removeLast();
		}
	}

	return result;
}

void benchmarkRemoval()
{
	// Data structures: array, vector, (bst = red-black tree i.e. bbst), tiered vector
	double* (*functions[AMOUNT])() = {
		&benchmarkRemovalArray
		, 
		&benchmarkRemovalVector
		,
		&benchmarkRemovalBST
		,
		&benchmarkRemovalK2TieredVector
		,
		&benchmarkRemovalBitTrickK2TieredVector
	};

	double results[AMOUNT][TESTS];
	for (int i = 0; i < AMOUNT; i++) {
		srand(SEED);
		double* result = functions[i]();
		cout << i << " done" << endl;
		for (int j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
	}
	return;
	ofstream outfile;
	outfile.open("removal.txt");
	outfile << "size;array;vector;bst;tiered vector;bittrick tiered vector\n";
	for (int i = 0; i < TESTS; i++) {
		outfile << INTERVAL * (TESTS - i);
		for (int j = 0; j < AMOUNT; j++) {
			outfile << ";" << results[j][i];
		}
		outfile << "\n";
	}
	outfile.close();
}

/*
	Access
*/

double* benchmarkAccessArray() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	for (int i = 0; i < TESTS; i++) {
		const int size = (i + 1) * INTERVAL;
		int *a = new int[size];

		double start = TIMER_FUNC();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % size;
			a[no];
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
		delete[] a;
	}
	return result;
}

double* benchmarkAccessVector() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	vector<int> v;

	for (int i = 0; i < TESTS; i++) {
		for (int j = 0; j < INTERVAL; j++) {
			v.push_back(j);
		}

		double start = TIMER_FUNC();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % v.size();
			v.at(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
	}
	return result;
}

double* benchmarkAccessBST() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	set<int> s;

	for (int i = 0; i < TESTS; i++) {
		for (int j = 0; j < INTERVAL; j++) {
			s.insert(i*INTERVAL + j);
		}

		double start = TIMER_FUNC();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % s.size();
			s.find(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
	}
	return result;
}

double* benchmarkAccessK2TieredVector() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	K2TieredVector tv;

	for (int i = 0; i < TESTS; i++) {
		for (int j = 0; j < INTERVAL; j++) {
			tv.insertLast(j);
		}

		double start = TIMER_FUNC();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % tv.n;
			tv.getElemAt(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
	}
	return result;
}

double* benchmarkAccessBitTrickK2TieredVector() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	BitTrickK2TieredVector tv;

	for (int i = 0; i < TESTS; i++) {
		for (int j = 0; j < INTERVAL; j++) {
			tv.insertLast(j);
		}

		double start = TIMER_FUNC();
		for (int j = 0; j < TESTS; j++) {
			int no = rand() % tv.n;
			tv.getElemAt(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
	}
	return result;
}

void benchmarkAccess()
{
	// Data structures: array, vector, (bst = red-black tree i.e. bbst), tiered vector
	double* (*functions[AMOUNT])() = {
		&benchmarkAccessArray
		,
		&benchmarkAccessVector
		,
		&benchmarkAccessBST
		,
		&benchmarkAccessK2TieredVector
		, 
		&benchmarkAccessBitTrickK2TieredVector
	};
	
	double results[AMOUNT][TESTS];
	for (int i = 0; i < AMOUNT; i++) {
		srand(SEED);
		double* result = functions[i]();
		cout << i << " done" << endl;
		for (int j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
	}

	ofstream outfile;
	outfile.open("access.txt");
	outfile << "size;array;vector;bst;tiered vector;bittrick tiered vector\n";
	for (int i = 0; i < TESTS; i++) {
		outfile << INTERVAL * i;
		for (int j = 0; j < AMOUNT; j++) {
			outfile << ";" << results[j][i];
		}
		outfile << "\n";
	}
	outfile.close();
}

//int main()
//{
//	//benchmarkInsertion();
//	//benchmarkRemoval();
//	//benchmarkAccess();
//	
//	string s;
//	cin >> s;
//	return 0;
//}