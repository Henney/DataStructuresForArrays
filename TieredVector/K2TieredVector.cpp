#include "stdafx.h"
#include "Deque.cpp"

class K2TieredVector : public TieredVector {
protected:
	Deque *a;

	void init(int size) {
		this->m = size;
		this->l = size;
		a = new Deque[size];
	}

	void doubleSize() {
		Deque *newA = new Deque[m * 2];
		for (int i = 0; i < m; i++) {
			newA[i] = a[i];
			newA[i].doubleSize();
		}
		for (int i = m; i < m * 2; i++) {
			delete[] (newA[i].a);
			newA[i] = Deque(l * 2);
		}
		delete[] a;
		a = newA;

		// Move everything to the lower 1/4 of the current TV
		int nextTvWithElems = 1;
		int moved = 0;
		int limit = n - a[0].n;
		for (int i = 0; moved < limit; i++) {
			while (!a[i].isFull()) {
				a[i].insertLast(a[nextTvWithElems].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (a[nextTvWithElems].isEmpty()) {
					nextTvWithElems = (nextTvWithElems + 1) % m;
				}
			}
		}
		m *= 2;
		l *= 2;
	}

	void halveSize() {
		Deque *newA = new Deque[m / 2];

		for (int i = 0; i < m / 2; i++) {
			delete[] (newA[i].a);
			newA[i] = Deque(l / 2);
		}

		for (int i = 0; i < m / 4; i++) {
			newA[i] = a[i / 2].lowerHalf();
			i++;
			newA[i] = a[i / 2].upperHalf();
		}
		delete[] a;
		a = newA;

		m /= 2;
		l /= 2;
	}

public:
	int m = 0;		// Number of subvectors

	K2TieredVector(int size) {
		init(size);
		for (int i = 0; i < size; i++) {
			delete[] (a[i].a);

			a[i] = Deque(size);
		}
	}

	K2TieredVector(void) {
		init(DEFAULT_SIZE);
	}

	~K2TieredVector(void) {
		for (int i = 0; i < m; i++) {
			delete[] (a[i].a);
		}
		delete[] a;
	}

	int getElemAt(int r) {
		int i = r / m;
		return a[i].getElemAt(r - i*l);
	}

	void insertElemAt(int r, int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = r / m;

		if (a[i].isFull()) {
			int back = n / m;
			if (a[back].isFull()) {
				back++;
			}
			for (int j = back; j > i; j--) {
				a[j].insertFirst(a[j - 1].removeLast());
			}
		}
		a[i].insertElemAt(r - i*m, e);
		n++;
	}

	void insertLast(int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = n / m;
		if (a[i].isFull())
			i++;
		a[i].insertLast(e);
		n++;
	}

	int removeElemAt(int r) {
		if (n < m * l / 8) {
			halveSize();
		}

		int i = r / m;
		int e = a[i].removeElemAt(r - i*m);
		for (int j = i; j < (n-1) / m; j++) {
			a[j].insertLast(a[j + 1].removeFirst());
		}
		n--;
		return e;
	}

	int removeLast() {
		if (n < m * l / 8) {
			halveSize();
		}

		int i = (n - 1) / m;
		int e = a[i].removeElemAt(n - 1 - i*m);
		n--;
		return e;
	}

	bool isFull() {
		return a[m - 1].isFull();
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += a[0].toStringPretty();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + a[i].toStringPretty();
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


class BitTrickK2TieredVector : public K2TieredVector {
protected:
	BitTrickDeque *a;

	int shift;

	void doubleSize() {
		int oldM = m;
		m = m << 1;
		l = l << 1;

		BitTrickDeque *newA = new BitTrickDeque[m];
		for (int i = 0; i < oldM; i++) {
			newA[i] = a[i];
			newA[i].doubleSize();
		}
		for (int i = oldM; i < m; i++) {
			delete[](newA[i].a);
			newA[i] = BitTrickDeque(l);
		}
		delete[] a;
		a = newA;
		
		// Move everything to the lower 1/4 of the current TV
		int nextTvWithElems = 1;
		int moved = 0;
		int limit = n - a[0].n;
		for (int i = 0; moved < limit; i++) {
			while (!a[i].isFull()) {
				a[i].insertLast(a[nextTvWithElems].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (a[nextTvWithElems].isEmpty()) {
					nextTvWithElems = (nextTvWithElems + 1) & (m - 1);
				}
			}
		}
		shift++;
	}

	void halveSize() {
		m = m >> 1;
		l = l >> 1;

		BitTrickDeque *newA = new BitTrickDeque[m];

		for (int i = 0; i < m; i++) {
			delete[](newA[i].a);
			newA[i] = BitTrickDeque(l);
		}

		for (int i = 0; i < m >> 1; i++) {
			newA[i] = a[i >> 1].lowerHalf();
			i++;
			newA[i] = a[i >> 1].upperHalf();
		}
		delete[] a;
		a = newA;

		shift--;
	}

public:
	int l = 0;
	int m = 0;

	BitTrickK2TieredVector() {
		this->m = DEFAULT_SIZE;
		this->l = DEFAULT_SIZE;
		this->shift = (int)log2(DEFAULT_SIZE);
		this->a = new BitTrickDeque[DEFAULT_SIZE];
	}

	~BitTrickK2TieredVector(void) {
		for (int i = 0; i < m; i++) {
			delete[](a[i].a);
		}
		delete[] a;
	}

	int getElemAt(int r) {
		int i = r >> shift;
		return a[i].getElemAt(r - i*l);
	}

	void insertElemAt(int r, int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = r >> shift;

		if (a[i].isFull()) {
			int back = n >> shift;
			if (a[back].isFull()) {
				back++;
			}
			for (int j = back; j > i; j--) {
				a[j].insertFirst(a[j - 1].removeLast());
			}
		}
		a[i].insertElemAt(r - (i << shift), e);
		n++;
	}

	void insertLast(int e) {
		if (isFull()) {
			doubleSize();
		}

		int i = n >> shift;
		if (a[i].isFull())
			i++;
		a[i].insertLast(e);
		n++;
	}

	int removeElemAt(int r) {
		if (n < (m << shift) >> 3) {		// Divide by 8
			halveSize();
		}

		int i = r >> shift;
		int e = a[i].removeElemAt(r - (i << shift));
		for (int j = i; j < (n - 1) >> shift; j++) {
			a[j].insertLast(a[j + 1].removeFirst());
		}
		n--;
		return e;
	}

	int removeLast() {
		if (n < m * l >> 3) {		// Divide by 8
			halveSize();
		}

		int i = (n - 1) >> shift;
		int e = a[i].removeElemAt(n - 1 - i*m);
		n--;
		return e;
	}

	bool isFull() {
		return a[m - 1].isFull();
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += a[0].toStringPretty();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + a[i].toStringPretty();
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