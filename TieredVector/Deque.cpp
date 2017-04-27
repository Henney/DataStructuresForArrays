#include "stdafx.h"
#include "Util.h"

class TieredVector {
public:
	int n = 0;	// Amount of elements (size)
	int l = 0;	// Capacity/length of the current deque

	int operator[](int index)
	{
		return getElemAt(index);
	}

	virtual int getElemAt(int r) = 0;
	virtual void insertElemAt(int r, int e) = 0;
	virtual int removeElemAt(int r) = 0;
};

class Deque : public TieredVector {
protected:
	int h = 0;
	bool topLevel = false;

	void incH(int inc) {
		h = (h + l + inc) % l;
	}
public:
	int *a;		// The array

	void init(int capacity) {
		this->a = new int[capacity];
		this->l = capacity;
	}

	Deque(int capacity) {
		init(capacity);
	}

	Deque(bool isTopLevel) {
		topLevel = isTopLevel;
		init(DEFAULT_SIZE);
	}

	Deque() {
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

	Deque lowerHalf() {
		Deque b(l / 2);

		for (int i = 0; i < l / 2; i++) {
			b.insertElemAt(i, getElemAt(i));
		}

		return b;
	}

	Deque upperHalf() {
		Deque b(l / 2);

		for (int i = l / 2; i < l; i++) {
			b.insertElemAt(i - l/2, getElemAt(i));
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
		checkIndexOutOfBounds(r, n + 1, "insert", "CircularDeque");
		//if (toplevel && isfull()) {
		//	doublesize();
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
		checkIndexOutOfBounds(r, n, "remove", "CircularDeque");
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
		if (n < 0) {
			cout << "n is below 0 in CircularDeque!" << endl;
			string s;
			cin >> s;
		}
		return e;
	}

	int removeFirst() {
		int e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			cout << "n is below 0 in CircularDeque!" << endl;
			string s;
			cin >> s;
		}
		return e;
	}

	int removeLast() {
		int e = a[(h + n - 1) % l];
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			cout << "n is below 0 in CircularDeque!" << endl;
			string s;
			cin >> s;
		}
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
			s += to_string(a[h]);
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

class BitTrickDeque : public Deque {
private:
	int shift;
protected:
	void incH(int inc) {
		h = (h + l + inc) & (l - 1);
	}
public:
	int *a;		// The array
	int n = 0;	// Amount of elements (size)
	int l = 0;	// Capacity/length of the current deque

	void init(int capacity) {
		this->a = new int[capacity];
		this->l = capacity;
		shift = (int)log2(capacity);
	}

	BitTrickDeque(int capacity) {
		init(capacity);
	}

	BitTrickDeque() {
		init(DEFAULT_SIZE);
	}

	int getElemAt(int r) {
		return a[(h + r) & (l - 1)];
	}

	BitTrickDeque lowerHalf() {
		BitTrickDeque b(l >> 1);

		for (int i = 0; i < l >> 1; i++) {
			b.insertElemAt(i, getElemAt(i));
		}

		return b;
	}

	BitTrickDeque upperHalf() {
		BitTrickDeque b(l >> 1);

		for (int i = l >> 1; i < l; i++) {
			b.insertElemAt(i - (l >> 1), getElemAt(i));
		}

		return b;
	}

	void doubleSize() {
		int *b = new int[l << 1];
		for (int i = 0; i < n; i++) {
			b[i] = getElemAt(i);
		}
		this->a = b;
		l = l << 1;
		h = 0;

		shift++;
	}

	void halveSize() {
		int *b = new int[l >> 1];
		for (int i = 0; i < l >> 1; i++) {
			b[i] = getElemAt(i);
		}
		this->a = b;
		l = l >> 1;
		h = 0;
	}

	void insertElemAt(int r, int e) {
		checkIndexOutOfBounds(r, n + 1, "insert", "CircularDeque");
		if (r >= l - r) {
			for (int i = h + n; i > h + r; i--) {
				a[(i + l) & (l - 1)] = a[(i + l - 1) & (l - 1)];
			}
		}
		else {
			for (int i = h; i < h + r; i++) {
				a[(i + l - 1) & (l - 1)] = a[i & (l - 1)];
			}
			incH(-1);
		}
		a[(h + r) & (l - 1)] = e;
		n++;
	}

	void insertFirst(int e) {
		incH(-1);
		a[h] = e;
		n++;
	}

	void insertLast(int e) {
		a[(h + n) & (l - 1)] = e;
		n++;
	}

	int removeElemAt(int r) {
		checkIndexOutOfBounds(r, n, "remove", "CircularDeque");

		int e = a[(h + r) & (l - 1)];

		if (r < n - r) {
			for (int i = h + r; i > h; i--) {
				a[i & (l - 1)] = a[(i + l - 1) & (l - 1)];
			}
			incH(1);
		} else {
			for (int i = h + r; i < h + n; i++) {
				a[i & (l - 1)] = a[(i + l + 1) & (l - 1)];
			}
		}
		n--;
		h = isEmpty() ? 0 : h;
		if (n < 0) {
			cout << "n is below 0 in CircularDeque!" << endl;
			string s;
			cin >> s;
		}
		return e;
	}

	int removeFirst() {
		int e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			cout << "n is below 0 in CircularDeque!" << endl;
			string s;
			cin >> s;
		}
		return e;
	}

	int removeLast() {
		int e = a[(h + n - 1) & (l - 1)];
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			cout << "n is below 0 in CircularDeque!" << endl;
			string s;
			cin >> s;
		}
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
			s += to_string(a[h]);
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