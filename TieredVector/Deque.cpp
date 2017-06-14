#include "stdafx.h"

#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED
#include "Util.h"
#endif // ! _UTIL_INCLUDED

class Deque : public ArrayDataStructure {
	friend class Simple2TieredVector;
private:
	int32_t n = 0;
	int32_t h = 0;
	int32_t m = 0;	// Capacity/length of the current deque
	bool topLevel = false;

	vector<int32_t> a;		// The array

	void init(int32_t capacity, bool topLevel) {
		if ((capacity - 1) & capacity)
			throw invalid_argument("capacity must be a power of two");
		this->topLevel = topLevel;
		this->m = capacity;
		this->a = vector<int32_t>(m);
	}
	void incH(int8_t inc) {
		h = (h + m + inc) % m;
	}

	void doubleSize() {
		vector<int32_t> b(m * 2);
		for (int32_t i = 0; i < n; i++) {
			b[i] = getElemAt(i);
		}
		//delete &a;
		this->a = b;
		m *= 2;
		h = 0;
	}

	void halveSize() {
		vector<int32_t> b(m / 2);
		for (int32_t i = 0; i < m / 2; i++) {
			b[i] = getElemAt(i);
		}
		//delete &a;
		this->a = b;
		m /= 2;
		h = 0;
	}
public:
	Deque(int32_t capacity, bool topLevel) {
		init(capacity, topLevel);
	}

	Deque(int32_t capacity) {
		init(capacity, false);
	}

	Deque(bool topLevel) { // TODO: topLevel should not be exposed
		init(DEFAULT_SIZE, topLevel);
	}

	Deque() {
		init(DEFAULT_SIZE, false);
	}

	~Deque(void) {
		a.clear();
		vector<int32_t>().swap(a);
	}

	uint32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		return a[(h + r) % m];
	}

	void setElemAt(int32_t r, int32_t e) {
		a[(h + r) % m] = e;
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

	void insertElemAt(int32_t r, int32_t e) {
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
		if (topLevel && n < m / 4) {
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
		return e;
	}

	int32_t removeFirst() {
		int32_t e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		return e;
	}

	int32_t removeLast() {
		int32_t e = a[(h + n - 1) % m];
		h = isEmpty() ? 0 : h;
		n--;
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

class BitTrickDeque : public ArrayDataStructure {
	friend class BitTrickSimple2TieredVector;
	friend class UnamortisedBitTrickSimple2TieredVector;
private:
	int32_t n = 0;
	int32_t h = 0;
	int32_t m = 0;	// Capacity/length of the current deque
	int8_t shift;
	bool topLevel = false;

	vector<int32_t> a;		// The array

	void init(int32_t capacity, bool topLevel) {
		if ((capacity - 1) & capacity)
			throw invalid_argument("capacity must be a power of two");
		this->topLevel = topLevel;
		this->m = capacity;
		this->a = vector<int32_t>(m);
		shift = (int8_t)log2(m);
	}
	void incH(int8_t inc) {
		h = (h + m + inc) & (m - 1);
	}

	void doubleSize() {
		vector<int32_t> b(m << 1);
		for (int32_t i = 0; i < n; i++) {
			b[i] = getElemAt(i);
		}
		//delete[] a;
		this->a = b;
		m = m << 1;
		h = 0;

		shift++;
	}

	void halveSize() {
		vector<int32_t> b(m >> 1);
		for (int32_t i = 0; i < m >> 1; i++) {
			b[i] = getElemAt(i);
		}
		//delete[] a;
		this->a = b;
		m = m >> 1;
		h = 0;
	}
public:
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

	~BitTrickDeque(void) {
		a.clear();
		vector<int32_t>().swap(a);
	}

	uint32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		return a[(h + r) & (m - 1)];
	}

	void setElemAt(int32_t r, int32_t e) {
		a[(h + r) & (m - 1)] = e;
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

	void insertElemAt(int32_t r, int32_t e) {
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
		return e;
	}

	int32_t removeFirst() {
		int32_t e = a[h];
		incH(1);
		h = isEmpty() ? 0 : h;
		n--;
		return e;
	}

	int32_t removeLast() {
		int32_t e = a[(h + n - 1) & (m - 1)];
		h = isEmpty() ? 0 : h;
		n--;
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