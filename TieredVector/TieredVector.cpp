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
		return (h + m + r) % m;
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
		CircularDeque *newB = new CircularDeque[m * 2];
		for (int i = 0; i < m; i++) {
			newB[i] = a[elem(i)];
			newB[i].doubleSize();
		}
		for (int i = m; i < m * 2; i++) {
			newB[i] = CircularDeque(m * 2); // TODO: Slow af way of doing this.. (allocating memory then reallocating)
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
#if DEBUG
		cout << endl << "Done doubling..." << endl << toString() << endl << toStringPretty() << endl;
#endif
	}
public:
	int n = 0;		// Number of elements
	int m = 0;		// Capacity/length

	TieredVector(int k) {
		this->k = k;
		this->m = (int)pow(2, k);
		if (k > 2) {
			cerr << "k > 2 is not implemented yet";
			exit(0);
			//recA = new TieredVector[DEFAULT_SIZE]{ k - 1, k - 1, k - 1, k - 1 };
			delete[] a;
		} else {
			a = new CircularDeque[m];
			//delete[] a;
		}
	}

	~TieredVector(void) {
		if (k > 2) {
			//delete[] recA;
		} else {
			delete[] a;
		}
	}

	int getElemAt(int r) {
		checkIndexOutOfBounds(r, n, "get", "TieredVector");

		if (k > 2) {
			//int l0 = a[(h + r) % length].size;
			//int i = r - (int)ceil(l0 / size);
			//return i == 0 ? a[i].getElemAt((h + r) % length) : a[i].getElemAt((h + (r - (i - 1) * size - l0)) % length); // TODO
			cerr << "k > 2 not implemented";
			exit(0);
			return -1;
		} else {
			int i = a[elem((int)ceil(float(r) / m))].getElemAt(r - i*m);
			return i; // == 0 ? a[elem(i)].getElemAt(r) : a[elem(i)].getElemAt(r - (i - 1) * a[elem(0)].l - l0);
		}
	}

	void insertElemAt(int r, int e) {
#if DEBUG
		cout << endl << "Inserting at r: " << r << endl << toString() << endl << toStringPretty() << endl;
#endif
		//checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");

		if (isFull()) {
			doubleSize();
		}

		// TODO: Only below is not correct
		int l0 = a[elem(0)].n;
		int insertIdx = (int)ceil(float(r + 1 - l0) / a[elem(0)].l);
		
		int newR = insertIdx == 0 ? r : (r - l0) % m;
		
#if DEBUG
		cout << "insertIdx: " << insertIdx << " r: " << r << " newR: " << newR << endl;
#endif
		//if (r >= n - r) {
#if DEBUG
			cout << "Inserting from back" << endl;
#endif
			if (a[elem(insertIdx)].isFull()) {
				int back = (int)ceil(float(n - l0) / a[elem(0)].l);
				if (a[elem(back)].isFull()) {
					back++;
				}
#if DEBUG
				cout << "Back: " << back << endl;
#endif
				for (int i = back; i > insertIdx; i--) {
					a[elem(i)].insertFirst(a[elem(i - 1)].removeLast());
				}
			}
		//} else {
#if DEBUG
			cout << "Inserting from front" << endl;
#endif
			/*if (a[elem(0)].isFull()) {
				incH(-1);
				if (r > 0) {
					insertIdx++;
				}
			}
			if (a[elem(insertIdx)].isFull()) {
				for (int i = 0; i < insertIdx; i++) {
					a[elem(i)].insertLast(a[elem(i + 1)].removeFirst());
				}
				newR = (newR + m - 1) % m;
			}
		}*/
		a[elem(insertIdx)].insertElemAt(newR, e);
		n++;
	}

	int removeElemAt(int r) {
		checkIndexOutOfBounds(r, n, "remove", "TieredVector");
		int e = a[elem(r / a[elem(0)].l)].removeElemAt(r % a[elem(0)].l);
		if (true) { //r >= size - r) {			
			for (int i = r / m; i < (n - 1) / m; i++) {
				a[elem(i)].insertLast(a[elem(i + 1)].removeFirst());
			}
		} else {
			incH(1);
		}
		n--;
		return e;
	}

	bool isFull() {
		// TODO: Might need to be defined differently. Depends on where the next element is to be inserted.
		return (a[elem(0)].isFull() && !a[elem(m-1)].isEmpty()) || a[elem(m - 1)].isFull();
		//return n == l * a[elem(0)].l;
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
