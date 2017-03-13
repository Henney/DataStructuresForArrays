// TieredVector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CircularDeque.cpp"

class K2TieredVector {
private:
	CircularDeque *a;

	int elem(int r) {
		return (l + r) % l;
	}

	void init(int k, int size) {
		this->m = size;
		this->l = size;
		a = new CircularDeque[size];
	}

	void doubleSize() {
		CircularDeque *newA = new CircularDeque[m * 2];
		for (int i = 0; i < m; i++) {
			newA[i] = a[elem(i)];
			newA[i].doubleSize();
		}
		for (int i = m; i < m * 2; i++) {
			delete[] (newA[i].a);
			newA[i] = CircularDeque(l * 2);
		}
		delete[] a;
		a = newA;

		// Move everything to the lower 1/4 of the current TV
		int nextTvWithElems = 1;
		int moved = 0;
		int limit = n - a[elem(0)].n;
		for (int i = 0; moved < limit; i++) {
			while (!a[elem(i)].isFull()) {
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
	}

	void halveSize() {
		CircularDeque *newA = new CircularDeque[m / 2];

		for (int i = 0; i < m / 2; i++) {
			delete[] (newA[i].a);
			newA[i] = CircularDeque(l / 2);
		}

		for (int i = 0; i < m / 4; i++) {
			newA[i] = a[elem(i / 2)].lowerHalf();
			i++;
			newA[i] = a[elem(i / 2)].upperHalf();
		}
		delete[] a;
		a = newA;

		m /= 2;
		l /= 2;
	}

public:
	int n = 0;		// Number of elements
	int m = 0;		// Number of subvectors
	int l = 0;		// Size of subvectors

	K2TieredVector(int k, int size) {
		init(k, size);
		for (int i = 0; i < size; i++) {
			delete[] (a[i].a);
			a[i] = CircularDeque(size);
		}
	}

	K2TieredVector(int k) {
		init(k, DEFAULT_SIZE);
	}

	~K2TieredVector(void) {
		for (int i = 0; i < m; i++) {
			delete[] (a[i].a);
		}
		delete[] a;
	}

	int getElemAt(int r) {
		int i = r / m;
		return a[elem(i)].getElemAt(r - i*m);
	}

	void insertElemAt(int r, int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = r / m;

		if (a[elem(i)].isFull()) {
			int back = n / m;
			if (a[elem(back)].isFull()) {
				back++;
			}
			for (int j = back; j > i; j--) {
				a[elem(j)].insertFirst(a[elem(j - 1)].removeLast());
			}
		}
		a[elem(i)].insertElemAt(r - i*m, e);
		n++;
	}

	void insertLast(int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = n / m;
		if (a[elem(i)].isFull())
			i++;
		a[elem(i)].insertLast(e);
		n++;
	}

	int removeElemAt(int r) {
		if (n < m * l / 8) {
			halveSize();
		}

		int i = (r-1) / m;
		int e = a[elem(i)].removeElemAt(r - i*m);
		for (int j = i; j < (n-1) / m; j++) {
			a[elem(j)].insertLast(a[elem(j + 1)].removeFirst());
		}
		n--;
		return e;
	}

	int removeLast() {
		if (n < m * l / 8) {
			halveSize();
		}

		int i = (n - 1) / m;
		int e = a[elem(i)].removeElemAt(n - 1 - i*m);
		n--;
		return e;
	}

	bool isFull() {
		return a[elem(m - 1)].isFull();
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += a[elem(0)].toStringPretty();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + a[elem(i)].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += a[0].toString();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + a[i].toString();
		}
		return s += " }";
	}
};

// TODO
class BitTrickK2TieredVector {
private:
	CircularDeque *a;

	int elem(int r) {
		return (l + r) % l;
	}

	void init(int k, int size) {
		this->m = size;
		this->l = size;
		a = new CircularDeque[size];
	}

	void doubleSize() {
		CircularDeque *newA = new CircularDeque[m * 2];
		for (int i = 0; i < m; i++) {
			newA[i] = a[elem(i)];
			newA[i].doubleSize();
		}
		for (int i = m; i < m * 2; i++) {
			delete[](newA[i].a);
			newA[i] = CircularDeque(l * 2);
		}
		delete[] a;
		a = newA;

		// Move everything to the lower 1/4 of the current TV
		int nextTvWithElems = 1;
		int moved = 0;
		int limit = n - a[elem(0)].n;
		for (int i = 0; moved < limit; i++) {
			while (!a[elem(i)].isFull()) {
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
	}

	void halveSize() {
		CircularDeque *newA = new CircularDeque[m / 2];

		for (int i = 0; i < m / 2; i++) {
			delete[](newA[i].a);
			newA[i] = CircularDeque(l / 2);
		}

		for (int i = 0; i < m / 4; i++) {
			newA[i] = a[elem(i / 2)].lowerHalf();
			i++;
			newA[i] = a[elem(i / 2)].upperHalf();
		}
		delete[] a;
		a = newA;

		m /= 2;
		l /= 2;
	}

public:
	int n = 0;		// Number of elements
	int m = 0;		// Number of subvectors
	int l = 0;		// Size of subvectors

	BitTrickK2TieredVector(int k, int size) {
		init(k, size);
		for (int i = 0; i < size; i++) {
			delete[](a[i].a);
			a[i] = CircularDeque(size);
		}
	}

	BitTrickK2TieredVector(int k) {
		init(k, DEFAULT_SIZE);
	}

	~BitTrickK2TieredVector(void) {
		for (int i = 0; i < m; i++) {
			delete[](a[i].a);
		}
		delete[] a;
	}

	int getElemAt(int r) {
		int i = r / m;
		return a[elem(i)].getElemAt(r - i*m);
	}

	void insertElemAt(int r, int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = r / m;

		if (a[elem(i)].isFull()) {
			int back = n / m;
			if (a[elem(back)].isFull()) {
				back++;
			}
			for (int j = back; j > i; j--) {
				a[elem(j)].insertFirst(a[elem(j - 1)].removeLast());
			}
		}
		a[elem(i)].insertElemAt(r - i*m, e);
		n++;
	}

	void insertLast(int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = n / m;
		if (a[elem(i)].isFull())
			i++;
		a[elem(i)].insertLast(e);
		n++;
	}

	int removeElemAt(int r) {
		if (n < m * l / 8) {
			halveSize();
		}

		int i = (r - 1) / m;
		int e = a[elem(i)].removeElemAt(r - i*m);
		for (int j = i; j < (n - 1) / m; j++) {
			a[elem(j)].insertLast(a[elem(j + 1)].removeFirst());
		}
		n--;
		return e;
	}

	int removeLast() {
		if (n < m * l / 8) {
			halveSize();
		}

		int i = (n - 1) / m;
		int e = a[elem(i)].removeElemAt(n - 1 - i*m);
		n--;
		return e;
	}

	bool isFull() {
		return a[elem(m - 1)].isFull();
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += a[elem(0)].toStringPretty();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + a[elem(i)].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += a[0].toString();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + a[i].toString();
		}
		return s += " }";
	}
};
