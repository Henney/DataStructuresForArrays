#include "stdafx.h"
#include "Util.h"
//#include "Util.cpp"

class CircularDeque {
private:
	int h = 0;
	bool topLevel = false;

	void incH(int inc) {
		h = (h + l + inc) % l;
	}
public:
	int *a;		// The array
	int n = 0;	// Amount of elements (size)
	int l = 0;	// Capacity/length of the current deque

	void init(int capacity) {
		this->a = new int[capacity];
		this->l = capacity;
	}

	CircularDeque(int capacity) {
		init(capacity);
	}

	CircularDeque(bool isTopLevel) {
		topLevel = isTopLevel;
		init(DEFAULT_SIZE);
	}

	CircularDeque() {
		init(DEFAULT_SIZE);
	}

	int getElemAt(int r) {
		return a[(h + r) % l];
	}

	void doubleSize() {
		int *b = new int[l * 2];
		for (int i = 0; i < n; i++) {
			b[i] = getElemAt(i);
		}
		this->a = b;
		l *= 2;
		h = 0;
	}

	CircularDeque lowerHalf() {
		CircularDeque b(l / 2);
		if (h < l / 2) {
			b.a = &a[h];
		} else {
			for (int i = 0; i < l / 2; i++) {
				b.insertElemAt(i, getElemAt(i));
			}
		}
		return b;
	}

	CircularDeque upperHalf() {
		CircularDeque b(l / 2);
		if (h + n < l) {
			b.a = &a[h + l/2];
		}
		else {
			for	(int i = 0; i < l / 2; i++) {
				b.insertElemAt(i, getElemAt(i + l / 2));
			}
		}

		return b;
	}

	void halveSize() {
		int *b = new int[l / 2];
		for (int i = 0; i < l / 2; i++) {
			b[i] = getElemAt(i);
		}
		this->a = b;
		l /= 2;
		h = 0;
	}

	void insertElemAt(int r, int e) {
		//if (topLevel && isFull()) {
		//	doubleSize();
		//}

		if (r >= l - r) {
			for (int i = h + n; i > h + r; i--) {
				a[(i + l) % l] = a[(i + l - 1) % l];
			}
		} else {
			for (int i = h; i < h + r; i++) {
				a[(i + l - 1) % l] = a[i % l];
			}
			incH(-1);
		}
		a[(h + r) % l] = e;
		n++;
	}

	void insertFirst(int e) {
		incH(-1);
		a[h] = e;
		n++;
	}

	void insertLast(int e) {
		a[(h + n) % l] = e;
		n++;
	}

	int removeElemAt(int r) {
		//if (topLevel && n < l / 4) {
		//	halveSize();
		//}

		int e = a[(h + r) % l];

		if (r < n - r) {
			for (int i = h + r; i > h; i--) {
				a[i % l] = a[(i + l - 1) % l];
			}
			incH(1);
		} else {
			for (int i = h + r; i < h + n; i++) {
				a[i % l] = a[(i + l + 1) % l];
			}
		}
		n--;
		h = isEmpty() ? 0 : h;
		return e;
	}

	int removeFirst() {
		int e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		return e;
	}

	int removeLast() {
		int e = a[(h + n - 1) % l];
		h = isEmpty() ? 0 : h;
		n--;
		return e;
	}

	bool isEmpty() {
		return 0 == n;
	}

	bool isFull() {
		return n == l;
	}

	string toStringPretty() {
		string s = "{ ";
		if (n > 0) {
			s += to_string(a[h % l]);
		}
		for (int i = 1; i < n; i++) {
			s += ", " + to_string(a[(i + h) % l]);
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		s += to_string(a[0]);
		for (int i = 1; i < l; i++) {
			s += ", " + to_string(a[i]);
		}
		return s += " }";
	}
};