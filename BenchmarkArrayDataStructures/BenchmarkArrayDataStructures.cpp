// BenchmarkArrayDataStructures.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>

#include "..\TieredVector\K2TieredVector.cpp"
#include "..\TieredVector\TieredVector.cpp"
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "util.h"

linear_congruential_engine<uint32_t, 48271, 0, 2147483647> rand_engine(1);
#define RAND_FUNC rand_engine
#define RESEED(x) rand_engine.seed(x)


#define MIN(a, b) (a < b ? a : b)
#define SEED 3938
#define TIMER_FUNC get_wall_time

#define TESTS 1000
#define INSERTIONS 500
#define DELETIONS 500
#define ACCESSES 100000
#define INTERVAL 500
#define AMOUNT 9

#define HEADERS "size;array;vector;bst;1-tiered vector;bit trick 1-tiered vector;simple 2-tiered vector;bit trick simple 2-tiered vector;deque 2-tiered vector;bit trick deque 2-tiered vector;\n"
#define DIR "../../results/"

#define ARRAY_AMOUNT 500000

/*
Insertion
*/

double* benchmarkInsertionArray() {
	int32_t maxElems = MIN(ARRAY_AMOUNT, INTERVAL*TESTS); // Takes too long otherwise
	int32_t tests = maxElems / INTERVAL;
	double* result = new double[TESTS];

	int32_t l = 1;
	int32_t *a = new int32_t[l];
	for (int32_t i = 0; i < tests; i++) {
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < INSERTIONS; j++) {
			int32_t no = RAND_FUNC() % (l + 1);
			a = arrayInsert(a, l, no, j);
			l++;
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int32_t j = 0; j < INTERVAL - INSERTIONS; j++) {
			a = arrayInsert(a, l, l, j);
			l++;
		}
	}
	delete[] a;
	return result;
}

double* benchmarkInsertionVector() {
	vector<int32_t> v;
	double* result = new double[TESTS];

	for (int32_t i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < INSERTIONS; j++) {
			int32_t no = RAND_FUNC() % (v.size() + 1);
			v.insert(v.begin() + no, j);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int32_t j = 0; j < INTERVAL - INSERTIONS; j++) {
			v.push_back(j);
		}
	}
	return result;
}


double* benchmarkInsertionBST() {
	set<int32_t> s;
	double* result = new double[TESTS];

	int32_t total = TESTS * INTERVAL;
	int32_t* numbers = (int32_t*)calloc(total, sizeof 0);
	for (int32_t i = 0; i < total; i++) {
		numbers[i] = i;
	}
	random_shuffle(numbers, numbers + total); // Shuffle the numbers

	for (int32_t i = 0; i < TESTS; i++) {
		int32_t elems = s.size();
		double start = TIMER_FUNC();
		for (int32_t j = elems; j < elems + INSERTIONS; j++) {
			s.insert(numbers[j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		elems = s.size();
		for (int32_t j = elems; j < elems + INTERVAL - INSERTIONS; j++) {
			s.insert(numbers[j]);
		}
	}
	return result;
}

double* benchmarkInsertion(ArrayDataStructure &ds) {
	double* result = new double[TESTS];
	for (int32_t i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < INSERTIONS; j++) {
			int32_t no = RAND_FUNC() % (ds.size() + 1);
			ds.insertElemAt(no, j);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int32_t j = 0; j < INTERVAL - INSERTIONS; j++) {
			ds.insertLast(j);
		}
	}
	return result;
}

double* benchmarkInsertionDeque() {
	Deque deque(true);
	return benchmarkInsertion(deque);
}

double* benchmarkInsertionBitTrickDeque() {
	BitTrickDeque deque(true);
	return benchmarkInsertion(deque);
}

double* benchmarkInsertion2TieredVector() {
	Simple2TieredVector tv;
	return benchmarkInsertion(tv);
}

double* benchmarkInsertionBitTrick2TieredVector() {
	BitTrickSimple2TieredVector tv;
	return benchmarkInsertion(tv);
}

double* benchmarkInsertionDeque2TieredVector() {
	Deque2TieredVector tv;
	return benchmarkInsertion(tv);
}

double* benchmarkInsertionBitTrickDeque2TieredVector() {
	BitTrickDeque2TieredVector tv;
	return benchmarkInsertion(tv);
}

void benchmarkInsertion()
{
	double* (*functions[AMOUNT])() = {
		&benchmarkInsertionArray
		,
		&benchmarkInsertionVector
		,
		&benchmarkInsertionBST
		,
		&benchmarkInsertionDeque
		,
		&benchmarkInsertionBitTrickDeque
		,
		&benchmarkInsertion2TieredVector
		,
		&benchmarkInsertionBitTrick2TieredVector
		,
		&benchmarkInsertionDeque2TieredVector
		,
		&benchmarkInsertionBitTrickDeque2TieredVector
	};

	double results[AMOUNT][TESTS];
	for (int32_t i = 0; i < AMOUNT; i++) {
		RESEED(SEED);
		double* result = functions[i]();
		cout << i << " done" << endl;
		for (int32_t j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
	}

	ofstream outfile;
	outfile.open(DIR "insertion.txt");
	outfile << HEADERS;
	for (int32_t i = 0; i < TESTS; i++) {
		outfile << INTERVAL * i;
		for (int32_t j = 0; j < AMOUNT; j++) {
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
	int32_t maxElems = MIN(ARRAY_AMOUNT, INTERVAL*TESTS); // Takes too long otherwise
	int32_t tests = maxElems / INTERVAL;
	double* result = new double[TESTS];

	int32_t l = INTERVAL * tests;
	int32_t *a = new int32_t[l];

	for (int32_t i = 0; i < l; i++) {
		a[i] = i;
	}

	for (int32_t i = 0; i < tests; i++) {
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < DELETIONS; j++) {
			int32_t no = RAND_FUNC() % l;
			a = arrayRemove(a, l, no);
			l--;
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int32_t j = 0; j < INTERVAL - DELETIONS; j++) {
			a = arrayRemove(a, l, l - 1);
			l--;
		}
	}
	delete[] a;
	return result;
}

double* benchmarkRemovalVector() {
	vector<int32_t> v;
	double* result = new double[TESTS];

	for (int32_t i = 0; i < INTERVAL * TESTS; i++) {
		v.push_back(i);
	}

	for (int32_t i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < DELETIONS; j++) {
			int32_t no = RAND_FUNC() % v.size();
			v.erase(v.begin() + no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int32_t j = 0; j < INTERVAL - DELETIONS; j++) {
			v.pop_back();
		}
	}
	return result;
}

double* benchmarkRemovalBST() {
	set<int32_t> s;
	double* result = new double[TESTS];
	int32_t total = TESTS * INTERVAL;

	for (int32_t i = 0; i < total; i++) {
		s.insert(i);
	}

	int32_t* numbers = (int32_t*)calloc(total, sizeof 0);
	for (int32_t i = 0; i < total; i++) {
		numbers[i] = i;
	}
	random_shuffle(numbers, numbers + total); // Shuffle the numbers

	for (int32_t i = 0; i < TESTS; i++) {
		int32_t elems = s.size();
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < DELETIONS; j++) {
			s.erase(numbers[elems - j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		elems = s.size();
		for (int32_t j = 0; j < INTERVAL - DELETIONS; j++) {
			s.erase(numbers[elems - j]);
		}
	}
	return result;
}

double* benchmarkRemoval(ArrayDataStructure &ds) {
	double* result = new double[TESTS];

	for (int32_t i = 0; i < TESTS * INTERVAL; i++) {
		ds.insertLast(i);
	}

	for (int32_t i = 0; i < TESTS; i++) {
		double start = TIMER_FUNC();
		for (int32_t j = 0; j < DELETIONS; j++) {
			int32_t no = RAND_FUNC() % ds.size();
			ds.removeElemAt(no);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;

		for (int32_t j = 0; j < INTERVAL - DELETIONS; j++) {
			ds.removeLast();
		}
	}

	return result;
}

double* benchmarkRemovalDeque() {
	Deque deque(true);
	return benchmarkRemoval(deque);
}

double* benchmarkRemovalBitTrickDeque() {
	BitTrickDeque deque(true);
	return benchmarkRemoval(deque);
}

double* benchmarkRemoval2TieredVector() {
	Simple2TieredVector tv;
	return benchmarkRemoval(tv);
}

double* benchmarkRemovalBitTrick2TieredVector() {
	BitTrickSimple2TieredVector tv;
	return benchmarkRemoval(tv);
}

double* benchmarkRemovalDeque2TieredVector() {
	Deque2TieredVector tv;
	return benchmarkRemoval(tv);
}

double* benchmarkRemovalBitTrickDeque2TieredVector() {
	BitTrickDeque2TieredVector tv;
	return benchmarkRemoval(tv);
}

void benchmarkRemoval()
{
	double* (*functions[AMOUNT])() = {
		&benchmarkRemovalArray
		,
		&benchmarkRemovalVector
		,
		&benchmarkRemovalBST
		,
		&benchmarkRemovalDeque
		,
		&benchmarkRemovalBitTrickDeque
		,
		&benchmarkRemoval2TieredVector
		,
		&benchmarkRemovalBitTrick2TieredVector
		,
		&benchmarkRemovalDeque2TieredVector
		,
		&benchmarkRemovalBitTrickDeque2TieredVector
	};

	double results[AMOUNT][TESTS];
	for (int32_t i = 0; i < AMOUNT; i++) {
		RESEED(SEED);
		double* result = functions[i]();
		cout << i << " done" << endl;
		for (int32_t j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
	}

	ofstream outfile;
	outfile.open(DIR "removal.txt");
	outfile << HEADERS;
	for (int32_t i = 0; i < TESTS; i++) {
		outfile << INTERVAL * (TESTS - i);
		for (int32_t j = 0; j < AMOUNT; j++) {
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

	for (int32_t i = 0; i < TESTS; i++) {
		const int32_t size = (i + 1) * INTERVAL;
		int32_t *a = new int32_t[size];

		int* order = (int*)calloc(ACCESSES, sizeof 0);
		for (int32_t j = 0; j < ACCESSES; j++) {
			order[j] = RAND_FUNC() % size;
		}

		double start = TIMER_FUNC();
		for (int32_t j = 0; j < ACCESSES; j++) {
			a[order[j]];
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
		delete[] a;
		delete[] order;
	}
	return result;
}

double* benchmarkAccessVector() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	vector<int32_t> v;

	for (int32_t i = 0; i < TESTS; i++) {
		for (int32_t j = 0; j < INTERVAL; j++) {
			v.push_back(j);
		}

		int* order = (int*)calloc(ACCESSES, sizeof 0);
		for (int32_t j = 0; j < ACCESSES; j++) {
			order[j] = RAND_FUNC() % v.size();
		}

		double start = TIMER_FUNC();
		for (int32_t j = 0; j < ACCESSES; j++) {
			v.at(order[j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
		delete[] order;
	}
	return result;
}

double* benchmarkAccessBST() {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	set<int32_t> s;

	for (int32_t i = 0; i < TESTS; i++) {
		for (int32_t j = 0; j < INTERVAL; j++) {
			s.insert(i*INTERVAL + j);
		}

		int* order = (int*)calloc(ACCESSES, sizeof 0);
		for (int32_t j = 0; j < ACCESSES; j++) {
			order[j] = RAND_FUNC() % s.size();
		}

		double start = TIMER_FUNC();
		for (int32_t j = 0; j < ACCESSES; j++) {
			s.find(order[j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
		delete[] order;
	}
	return result;
}

double* benchmarkAccess(ArrayDataStructure &ds) {
	double* result = (double*)calloc(TESTS, sizeof 0.0);

	for (int32_t i = 0; i < TESTS; i++) {
		for (int32_t j = 0; j < INTERVAL; j++) {
			ds.insertLast(j);
		}

		int* order = (int*)calloc(ACCESSES, sizeof 0);
		for (int32_t j = 0; j < ACCESSES; j++) {
			order[j] = RAND_FUNC() % ds.size();
		}

		double start = TIMER_FUNC();
		for (int32_t j = 0; j < ACCESSES; j++) {
			ds.getElemAt(order[j]);
		}
		double end = TIMER_FUNC();
		result[i] = end - start;
		delete[] order;
	}
	return result;
}

double* benchmarkAccessDeque() {
	Deque deque(true);
	return benchmarkAccess(deque);
}

double* benchmarkAccessBitTrickDeque() {
	BitTrickDeque deque(true);
	return benchmarkAccess(deque);
}

double* benchmarkAccess2TieredVector() {
	Simple2TieredVector tv;
	return benchmarkAccess(tv);
}

double* benchmarkAccessBitTrick2TieredVector() {
	BitTrickSimple2TieredVector tv;
	return benchmarkAccess(tv);
}

double* benchmarkAccessDeque2TieredVector() {
	Deque2TieredVector tv;
	return benchmarkAccess(tv);
}

double* benchmarkAccessBitTrickDeque2TieredVector() {
	BitTrickDeque2TieredVector tv;
	return benchmarkAccess(tv);
}

void benchmarkAccess()
{
	double* (*functions[AMOUNT])() = {
		&benchmarkAccessArray
		,
		&benchmarkAccessVector
		,
		&benchmarkAccessBST
		,
		&benchmarkAccessDeque
		,
		&benchmarkAccessBitTrickDeque
		,
		&benchmarkAccess2TieredVector
		,
		&benchmarkAccessBitTrick2TieredVector
		,
		&benchmarkAccessDeque2TieredVector
		,
		&benchmarkAccessBitTrickDeque2TieredVector
	};

	double results[AMOUNT][TESTS];
	for (int32_t i = 0; i < AMOUNT; i++) {
		RESEED(SEED);
		double* result = functions[i]();
		cout << i << " done" << endl;
		for (int32_t j = 0; j < TESTS; j++) {
			results[i][j] = result[j];
		}
	}

	ofstream outfile;
	outfile.open(DIR "access_opt.txt");
	outfile << HEADERS;
	for (int32_t i = 0; i < TESTS; i++) {
		outfile << INTERVAL * i;
		for (int32_t j = 0; j < AMOUNT; j++) {
			outfile << ";" << results[j][i];
		}
		outfile << "\n";
	}
	outfile.close();
}

int main()
{
	//benchmarkInsertion();
	//benchmarkRemoval();
	//benchmarkAccess();

	string s;
	cin >> s;
	return 0;
}