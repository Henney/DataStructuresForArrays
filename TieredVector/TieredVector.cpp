// TieredVector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CircularDeque.cpp"

class TieredVector {
private:
	int k = 0;		// Tier of the current vector
	int shift = 0;	// TODO

	//TieredVector *recA;
	CircularDeque *a;

	int h = 0;		// Head element

	int elem(int r) {
		return (h + l + r) % l;
	}

	void init(int k, int size) {
		this->k = k;
		this->m = size;
		this->l = size;
		this->shift = 1;
		a = new CircularDeque[m];
	}

	void doubleSize() {
#if DEBUG
		cout << endl << "Doubling..." << endl << toString() << endl << toStringPretty() << endl;
#endif
		CircularDeque *newB = new CircularDeque[m * 2];
		for (int i = 0; i < m; i++) {
			newB[i] = a[elem(i)];
			newB[i].doubleSize();
		}
		for (int i = m; i < m * 2; i++) {
			newB[i] = CircularDeque(l * 2); // TODO: Slow way of doing this.. (allocating memory then reallocating)
		}
		a = newB;
		h = 0;

		// Move everything to the lower 1/4 of the current TV
		int nextTvWithElems = 1;
		int moved = 0;
		int limit = n - a[elem(0)].n;
		for (int i = 0; moved < limit; i++) { // TODO: Not very efficient?
			while (!a[elem(i)].isFull()) { // TODO: inBounds check not necessary?
				a[elem(i)].insertLast(a[elem(nextTvWithElems)].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (a[elem(nextTvWithElems)].isEmpty()) {
					nextTvWithElems = (nextTvWithElems + 1) % m;
				}
			}
		}
		m *= 2;
		l *= 2;
#if DEBUG
		cout << endl << "Done doubling..." << endl << toString() << endl << toStringPretty() << endl;
#endif
	}
public:
	int n = 0;		// Number of elements
	int m = 0;		// Number of subvectors
	int l = 0;		// Size of subvectors

	TieredVector(int k, int size) {
		init(k, size);
	}

	TieredVector(int k) {
		init(k, DEFAULT_SIZE);
	}

	~TieredVector(void) {
		delete[] a;
	}

	int getElemAt(int r) {
		int i = r / m;
		return a[elem(i)].getElemAt(r - i*m);
	}

	void insertElemAt(int r, int e) {
#if DEBUG
		cout << endl << "Inserting e: " << e << " at r: " << r << endl << toString() << endl << toStringPretty() << endl;
#endif
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
		if (isFull()) {
			doubleSize();
		}

		int i = r / m;

#if DEBUG
		cout << "insertIdx: " << i << " r: " << r << " newR: " << r - i*m << endl;
#endif
		if (a[elem(i)].isFull()) {
			int back = n / m;
			if (a[elem(back)].isFull()) {
				back++;
			}
#if DEBUG
			cout << "Back: " << back << endl;
#endif
			for (int j = back; j > i; j--) {
				a[elem(j)].insertFirst(a[elem(j - 1)].removeLast());
			}
		}
		a[elem(i)].insertElemAt(r - i*m, e);
		n++;
	}

	int removeElemAt(int r) {
		checkIndexOutOfBounds(r, n, "remove", "TieredVector");
		int i = r / m;
		int e = a[elem(i)].removeElemAt(r - i*m);
		for (int j = i; j < (n-1) / m; j++) {
			a[elem(j)].insertLast(a[elem(j + 1)].removeFirst());
		}
		n--;
		return e;
	}

	bool isFull() {
		return a[elem(m - 1)].isFull();
	}

	string toStringPretty() {
		string s = "{ ";
		if (k > 2) {
		}
		else {
			if (m > 0) {
				s += a[elem(0)].toStringPretty();
			}
			for (int i = 1; i < m; i++) {
				s += ", " + a[elem(i)].toStringPretty();
			}
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (k > 2) {
		}
		else {
			if (m > 0) {
				s += a[0].toString();
			}
			for (int i = 1; i < m; i++) {
				s += ", " + a[i].toString();
			}
		}
		return s += " }";
	}
};
