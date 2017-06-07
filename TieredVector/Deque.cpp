#include "stdafx.h"

#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED
#include "Util.h"
#endif // ! _UTIL_INCLUDED

class Deque : public RankSequence {
private:
	int32_t h = 0;
	int32_t m = 0;	// Capacity/length of the current deque
	bool topLevel = false;

	void init(int32_t capacity, bool topLevel) {
		if ((capacity - 1) & capacity)
			throw invalid_argument("capacity must be a power of two");
		this->topLevel = topLevel;
		this->a = new int32_t[capacity];
		this->m = capacity;
	}
	void incH(int8_t inc) {
		h = (h + m + inc) % m;
	}
public:
	int32_t* a;		// The array

	Deque(int32_t capacity, bool topLevel) {
		init(capacity, topLevel);
	}

	Deque(int32_t capacity) {
		init(capacity, false);
	}

	Deque(bool topLevel) {
		init(DEFAULT_SIZE, topLevel);
	}

	Deque() {
		init(DEFAULT_SIZE, false);
	}

	int32_t getElemAt(int32_t r) {
		return a[(h + r) % m];
	}

	void doubleSize() {
		int32_t *b = new int32_t[m * 2];
		for (int32_t i = 0; i < n; i++) {
			b[i] = getElemAt(i);
		}
		delete[] a;
		this->a = b;
		m *= 2;
		h = 0;
	}

	Deque lowerHalf() {
		Deque b(m / 2);

		for (int32_t i = 0; i < m / 2; i++) {
			b.insertElemAt(i, getElemAt(i));
		}

		return b;
	}

	Deque upperHalf() {
		Deque b(m / 2);

		for (int32_t i = m / 2; i < m; i++) {
			b.insertElemAt(i - m/2, getElemAt(i));
		}

		return b;
	}

	void halveSize() {
		int32_t *b = new int32_t[m / 2];
		for (int32_t i = 0; i < m / 2; i++) {
			b[i] = getElemAt(i);
		}
		delete[] a;
		this->a = b;
		m /= 2;
		h = 0;
	}

	void insertElemAt(int32_t r, int32_t e) {
		checkIndexOutOfBounds(r, n + 1, "insert", "CircularDeque");
		if (isFull()) {
			doubleSize();
		}

		if (r >= m - r) {
			for (int32_t i = h + n; i > h + r; i--) {
				a[(i + m) % m] = a[(i + m - 1) % m];
			}
		} else {
			for (int32_t i = h; i < h + r; i++) {
				a[(i + m - 1) % m] = a[i % m];
			}
			incH(-1);
		}
		a[(h + r) % m] = e;
		n++;
	}

	void insertFirst(int32_t e) {
		if (isFull()) {
			doubleSize();
		}
		incH(-1);
		a[h] = e;
		n++;
	}

	void insertLast(int32_t e) {
#if DEBUG
		cout << "Adding element " << e << " last in Deque. n: " << n << endl;
#endif
		if (isFull()) {
			doubleSize();
		}
		a[(h + n) % m] = e;
		n++;
	}

	int32_t removeElemAt(int32_t r) {
#if DEBUG
		checkIndexOutOfBounds(r, n, "remove", "CircularDeque");
		cout << "removing at " << r << ". n: " << n << endl;
#endif
		if (topLevel && n < m / 4) {
#if DEBUG
			cout << "n: " << n << endl;
#endif
			halveSize();
		}

		int32_t e = a[(h + r) % m];

		if (r < n - r) {
			for (int32_t i = h + r; i > h; i--) {
				a[i % m] = a[(i + m - 1) % m];
			}
			incH(1);
		} else {
			for (int32_t i = h + r; i < h + n; i++) {
				a[i % m] = a[(i + m + 1) % m];
			}
		}
		n--;
		h = isEmpty() ? 0 : h;
		if (n < 0) {
			warnNBelow0();
		}
		return e;
	}

	int32_t removeFirst() {
		int32_t e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			warnNBelow0();
		}
		return e;
	}

	int32_t removeLast() {
		int32_t e = a[(h + n - 1) % m];
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			warnNBelow0();
		}
		return e;
	}

	bool isEmpty() {
		return 0 == n;
	}

	bool isFull() {
		return n == m;
	}

	string toStringSimple() {
		string s = "";
		if (n > 0) {
			s += to_string(a[h]);
		}
		for (int32_t i = 1; i < n; i++) {
			s += ", " + to_string(a[(i + h) % m]);
		}
		return s;
	}

	string toStringPretty() {
		string s = "{ ";
		if (n > 0) {
			s += to_string(a[h]);
		}
		for (int32_t i = 1; i < n; i++) {
			s += ", " + to_string(a[(i + h) % m]);
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		s += to_string(a[0]);
		for (int32_t i = 1; i < m; i++) {
			s += ", " + to_string(a[i]);
		}
		return s += " }";
	}
};

class BitTrickDeque : public RankSequence {
private:
	int32_t h = 0;
	int8_t shift;
	bool topLevel = false;

	void init(int32_t capacity, bool topLevel) {
		if ((capacity - 1) & capacity)
			throw invalid_argument("capacity must be a power of two");
		this->topLevel = topLevel;
		this->a = new int32_t[capacity];
		this->m = capacity;
		shift = (int8_t)log2(capacity);
	}
	void incH(int8_t inc) {
		h = (h + m + inc) & (m - 1);
	}
public:
	int32_t *a;		// The array
	int32_t m = 0;	// Capacity/length of the current deque

	BitTrickDeque(int32_t capacity, bool topLevel) {
		init(capacity, topLevel);
	}

	BitTrickDeque(int32_t capacity) {
		init(capacity, false);
	}

	BitTrickDeque(bool topLevel) {
		init(DEFAULT_SIZE, topLevel);
	}

	BitTrickDeque() {
		init(DEFAULT_SIZE, false);
	}

	int32_t getElemAt(int32_t r) {
		return a[(h + r) & (m - 1)];
	}

	BitTrickDeque lowerHalf() {
		BitTrickDeque b(m >> 1);

		for (int32_t i = 0; i < m >> 1; i++) {
			b.insertElemAt(i, getElemAt(i));
		}

		return b;
	}

	BitTrickDeque upperHalf() {
		BitTrickDeque b(m >> 1);

		for (int32_t i = m >> 1; i < m; i++) {
			b.insertElemAt(i - (m >> 1), getElemAt(i));
		}

		return b;
	}

	void doubleSize() {
		int32_t *b = new int32_t[m << 1];
		for (int32_t i = 0; i < n; i++) {
			b[i] = getElemAt(i);
		}
		delete[] a;
		this->a = b;
		m = m << 1;
		h = 0;

		shift++;
	}

	void halveSize() {
		int32_t *b = new int32_t[m >> 1];
		for (int32_t i = 0; i < m >> 1; i++) {
			b[i] = getElemAt(i);
		}
		delete[] a;
		this->a = b;
		m = m >> 1;
		h = 0;
	}

	void insertElemAt(int32_t r, int32_t e) {
#if DEBUG
		checkIndexOutOfBounds(r, n + 1, "insert", "CircularDeque");
#endif
		if (isFull()) {
			doubleSize();
		}

		if (r >= m - r) {
			for (int32_t i = h + n; i > h + r; i--) {
				a[(i + m) & (m - 1)] = a[(i + m - 1) & (m - 1)];
			}
		}
		else {
			for (int32_t i = h; i < h + r; i++) {
				a[(i + m - 1) & (m - 1)] = a[i & (m - 1)];
			}
			incH(-1);
		}
		a[(h + r) & (m - 1)] = e;
		n++;
	}

	void insertFirst(int32_t e) {
		if (isFull()) {
			doubleSize();
		}
		incH(-1);
		a[h] = e;
		n++;
	}

	void insertLast(int32_t e) {
		if (isFull()) {
			doubleSize();
		}
		a[(h + n) & (m - 1)] = e;
		n++;
	}

	int32_t removeElemAt(int32_t r) {
#if DEBUG
		checkIndexOutOfBounds(r, n, "remove", "CircularDeque");
#endif
		if (topLevel && n < m / 4) {
			halveSize();
		}

		int32_t e = a[(h + r) & (m - 1)];

		if (r < n - r) {
			for (int32_t i = h + r; i > h; i--) {
				a[i & (m - 1)] = a[(i + m - 1) & (m - 1)];
			}
			incH(1);
		} else {
			for (int32_t i = h + r; i < h + n; i++) {
				a[i & (m - 1)] = a[(i + m + 1) & (m - 1)];
			}
		}
		n--;
		h = isEmpty() ? 0 : h;
		if (n < 0) {
			warnNBelow0();
		}
		return e;
	}

	int32_t removeFirst() {
		int32_t e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			warnNBelow0();
		}
		return e;
	}

	int32_t removeLast() {
		int32_t e = a[(h + n - 1) & (m - 1)];
		h = isEmpty() ? 0 : h;
		n--;
		if (n < 0) {
			warnNBelow0();
		}
		return e;
	}

	bool isEmpty() {
		return 0 == n;
	}

	bool isFull() {
		return n == m;
	}

	string toStringSimple() {
		string s = "";
		if (n > 0) {
			s += to_string(a[h]);
		}
		for (int32_t i = 1; i < n; i++) {
			s += ", " + to_string(a[(i + h) & (m - 1)]);
		}
		return s;
	}

	string toStringPretty() {
		string s = "{ ";
		if (n > 0) {
			s += to_string(a[h]);
		}
		for (int32_t i = 1; i < n; i++) {
			s += ", " + to_string(a[(i + h) % m]);
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		s += to_string(a[0]);
		for (int32_t i = 1; i < m; i++) {
			s += ", " + to_string(a[i]);
		}
		return s += " }";
	}
};