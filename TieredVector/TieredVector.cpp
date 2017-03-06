// TieredVector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CircularDeque.cpp"

class TieredVector {
private:
	int k = 0;		// Tier of the current vector

	//TieredVector *recA;
	CircularDeque *a;

	int h = 0;		// Head element

	int elem(int r) {
		return (h + l + r) % l;
	}

	void incH(int inc) {
		h = (h + m + inc) % m;
	}

	bool inBounds(int x) {
		return x >= h || x < (h + n) % m;
	}

	void doubleSize() {
#if DEBUG
		cout << endl << "Doubling..." << endl << toString() << endl << toStringPretty() << endl;
#endif
		CircularDeque *newB = new CircularDeque[m << 1];
		for (int i = 0; i < m; i++) {
			newB[i] = a[elem(i)];
			newB[i].doubleSize();
		}
		for (int i = m; i < m << 1; i++) {
			newB[i] = CircularDeque(l << 1); // TODO: Slow af way of doing this.. (allocating memory then reallocating)
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
		m = m << 1;
		l = l << 1;
#if DEBUG
		cout << endl << "Done doubling..." << endl << toString() << endl << toStringPretty() << endl;
#endif
	}
public:
	int n = 0;		// Number of elements
	int m = 0;		// Number of subvectors
	int l = 0;		// Size of subvectors

	TieredVector(int k) {
		this->k = k;
		this->m = DEFAULT_SIZE;
		this->l = DEFAULT_SIZE;
		a = new CircularDeque[m];
	}

	~TieredVector(void) {
		delete[] a;
	}

	int getElemAt(int r) {
		checkIndexOutOfBounds(r, n, "get", "TieredVector");

		int i = (int)ceil(float(r) / m);
		return a[elem(i)].getElemAt(r - i*m);
	}

	void insertElemAt(int r, int e) {
#if DEBUG
		cout << endl << "Inserting at r: " << r << endl << toString() << endl << toStringPretty() << endl;
#endif
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");

		if (isFull()) {
			doubleSize();
		}

		int i = r / m;
		
#if DEBUG
		cout << "insertIdx: " << i << " r: " << r << " newR: " << r - i*m << endl;
		cout << "Inserting from back" << endl;
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
#if DEBUG
		cout << endl << "Removing at r: " << r << endl << toString() << endl << toStringPretty() << endl;
#endif
		checkIndexOutOfBounds(r, n, "remove", "TieredVector");
		int i = r / m;
		int e = a[elem(i)].removeElemAt(r - i*m);
		for (int j = i; j < (n-1) / m; j++) {
			a[elem(j)].insertLast(a[elem(j + 1)].removeFirst());
		}
		n--;
#if DEBUG
		cout << endl << "Removed: " << r << endl << toString() << endl << toStringPretty() << endl;
#endif
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
