#include "stdafx.h"
#include "Deque.cpp"

class K2TieredVector : public RankSequence {
private:
	int32_t n = 0;
	int32_t m = 0;	// Number of children
protected:
	Deque *children;

	void init(int32_t size) {
		this->m = size;
		children = new Deque[size];
	}

	void doubleSize() {
		int32_t oldM = m;
		m *= 2;

		Deque *newSubvectors = new Deque[m];
		for (int32_t i = 0; i < oldM; i++) {
			newSubvectors[i] = children[i];
			newSubvectors[i].doubleSize();
		}
		for (int32_t i = oldM; i < m; i++) {
			delete[](newSubvectors[i].a);
			newSubvectors[i] = Deque(m);
		}
		delete[] children;
		children = newSubvectors;

		// Move everything to the lower 1/4 of the current TV
		int32_t nextDequeWithElems = 1;
		int32_t moved = 0;
		int32_t limit = n - children[0].size();
		for (int32_t i = 0; moved < limit; i++) {
			while (!children[i].isFull()) {
				children[i].insertLast(children[nextDequeWithElems].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (children[nextDequeWithElems].isEmpty()) {
					nextDequeWithElems = (nextDequeWithElems + 1);
				}
			}
		}
	}

	void halveSize() {
		Deque *newA = new Deque[m / 2];

		for (int32_t i = 0; i < m / 2; i++) {
			delete[] (newA[i].a);
			newA[i] = Deque(m / 2);
		}

		for (int32_t i = 0; i < m / 4; i++) {
			newA[i] = children[i / 2].lowerHalf();
			i++;
			newA[i] = children[i / 2].upperHalf();
		}
		delete[] children;
		children = newA;

		m /= 2;
	}

public:
	K2TieredVector(int32_t size) {
		init(size);
		for (int32_t i = 0; i < size; i++) {
			delete[] (children[i].a);
			children[i] = Deque(size);
		}
	}

	K2TieredVector(void) {
		init(DEFAULT_SIZE);
	}

	~K2TieredVector(void) {
		for (int32_t i = 0; i < m; i++) {
			delete[] (children[i].a);
		}
		delete[] children;
	}

	int32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		int32_t i = r / m;
		return children[i].getElemAt(r - i*m);
	}

	void setElemAt(int32_t r, int32_t e) {
		int32_t i = r / m;
		return children[i].setElemAt(r - i*m, e);
	}

	void insertElemAt(int32_t r, int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = r / m;

		if (children[i].isFull()) {
			int32_t back = n / m;
			if (children[back].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				children[j].insertFirst(children[j - 1].removeLast());
			}
		}
		children[i].insertElemAt(r - i*m, e);
		n++;
	}

	void insertLast(int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = n / m;
		if (children[i].isFull())
			i++;
		children[i].insertLast(e);
		n++;
	}

	int32_t removeElemAt(int32_t r) {
#if DEBUG
		cout << "Removing at " << r << " in K2TieredVector. n: " << n << endl;
#endif
		if (n < m * m / 8) {
			halveSize();
		}

		int32_t i = r / m;
		int32_t e = children[i].removeElemAt(r - i*m);
		for (int32_t j = i; j < (n-1) / m; j++) {
			children[j].insertLast(children[j + 1].removeFirst());
		}
		n--;
		return e;
	}

	int32_t removeLast() {
		if (n < m * m / 8) {
			halveSize();
		}

		int32_t i = (n - 1) / m;
		int32_t e = children[i].removeElemAt(n - 1 - i*m);
		n--;
		return e;
	}

	bool isFull() {
		return children[m - 1].isFull();
	}

	string toStringSimple() {
		string s = "";
		if (m > 0) {
			s += children[0].toStringSimple();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toStringSimple();
		}
		return s;
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += children[0].toStringPretty();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += children[0].toString();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toString();
		}
		return s += " }";
	}
};


class BitTrickK2TieredVector : public RankSequence {
private:
	int32_t n = 0;
	int32_t m = 0;
protected:
	BitTrickDeque *children;

	int8_t shift;

	void doubleSize() {
		int32_t oldM = m;
		m = m << 1;

		BitTrickDeque *newSubvectors = new BitTrickDeque[m];
		for (int32_t i = 0; i < oldM; i++) {
			newSubvectors[i] = children[i];
			newSubvectors[i].doubleSize();
		}
		for (int32_t i = oldM; i < m; i++) {
			delete[](newSubvectors[i].a);
			newSubvectors[i] = BitTrickDeque(m);
		}
		delete[] children;
		children = newSubvectors;
		
		// Move everything to the lower 1/4 of the current TV
		int32_t nextDequeWithElems = 1;
		int32_t moved = 0;
		int32_t limit = n - children[0].size();
		for (int32_t i = 0; moved < limit; i++) {
			while (!children[i].isFull()) {
				children[i].insertLast(children[nextDequeWithElems].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (children[nextDequeWithElems].isEmpty()) {
					nextDequeWithElems = (nextDequeWithElems + 1);
				}
			}
		}
		shift++;
	}

	void halveSize() {
		m = m >> 1;

		BitTrickDeque *newSubvectors = new BitTrickDeque[m];

		for (int32_t i = 0; i < m; i++) {
			delete[](newSubvectors[i].a);
			newSubvectors[i] = BitTrickDeque(m);
		}

		for (int32_t i = 0; i < m >> 1; i++) {
			newSubvectors[i] = children[i >> 1].lowerHalf();
			i++;
			newSubvectors[i] = children[i >> 1].upperHalf();
		}
		delete[] children;
		children = newSubvectors;

		shift--;
	}

public:
	BitTrickK2TieredVector() {
		this->m = DEFAULT_SIZE;
		this->shift = (int32_t)log2(DEFAULT_SIZE);
		this->children = new BitTrickDeque[DEFAULT_SIZE];
	}

	~BitTrickK2TieredVector(void) {
		for (int32_t i = 0; i < m; i++) {
			delete[](children[i].a);
		}
		delete[] children;
	}

	int32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		int32_t i = r >> shift;
		return children[i].getElemAt(r - (i << shift));
	}

	void setElemAt(int32_t r, int32_t e) {
		int32_t i = r >> shift;
		return children[i].setElemAt(r - (i << shift), e);
	}

	void insertElemAt(int32_t r, int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = r >> shift;

		if (children[i].isFull()) {
			int32_t back = n >> shift;
			if (children[back].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				children[j].insertFirst(children[j - 1].removeLast());
			}
		}
		children[i].insertElemAt(r - (i << shift), e);
		n++;
	}

	void insertLast(int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = n >> shift;
		if (children[i].isFull())
			i++;
		children[i].insertLast(e);
		n++;
	}

	int32_t removeElemAt(int32_t r) {
#if DEBUG
		cout << "Removing at " << r << " in BitTrickK2TieredVector. n: " << n << endl;
#endif
		if (n < (m << shift) >> 3) {		// Divide by 8
			halveSize();
		}

		int32_t i = r >> shift;
		int32_t e = children[i].removeElemAt(r - (i << shift));
		for (int32_t j = i; j < (n - 1) >> shift; j++) {
			children[j].insertLast(children[j + 1].removeFirst());
		}
		n--;
		return e;
	}

	int32_t removeLast() {
		if (n < m << shift >> 3) {		// Divide by 8
			halveSize();
		}

		int32_t i = (n - 1) >> shift;
		int32_t e = children[i].removeElemAt(n - 1 - (i << shift));
		n--;
		return e;
	}

	bool isFull() {
		return children[m - 1].isFull();
	}

	string toStringSimple() {
		string s = "";
		if (m > 0) {
			s += children[0].toStringSimple();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toStringSimple();
		}
		return s;
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += children[0].toStringPretty();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += children[0].toString();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toString();
		}
		return s += " }";
	}
};

// TODO: Make a version that utilises bitshifts
class DequeK2TieredVector : public RankSequence {
private:
	int32_t n = 0;
	int32_t h = 0;
	int32_t m = 0;		// Number of children

	Deque *children;

	int32_t elem(int32_t r) {
		return (h + r) % m;
	}

	void incH(int8_t inc) {
		h = (h + inc + m) % m;
	}

	void init(int32_t size) {
		this->m = size;
		children = new Deque[size];
	}

	void doubleSize() {
		Deque *newA = new Deque[m * 2];
		for (int32_t i = 0; i < m * 2; i++) {
			delete[](newA[i].a);
			newA[i] = Deque(m * 2);
		}

		// Move everything to the lower 1/4 of this tiered vector
		int32_t idx = 0;
		for (int32_t i = 0; i < m; i++) {
			while (!newA[i].isFull() && idx < n) {
				newA[i].insertLast(getElemAt(idx));
				idx++;
			}
		}
		delete[] children;
		children = newA;

		m *= 2;
		h = 0;
	}

	void halveSize() {
		Deque *newA = new Deque[m / 2];

		for (int32_t i = 0; i < m / 2; i++) {
			delete[](newA[i].a);
			newA[i] = Deque(m / 2);
		}

		int32_t to = 0;
		for (int32_t i = 0; i < n; i++) {
			newA[to].insertElemAt(i % (m / 2), getElemAt(i));
			if (newA[to].isFull())
				to++;
		}

		delete[] children;
		children = newA;

		m /= 2;
		h = 0;
	}

	bool lastFull() {
		int32_t i = (h - 1 + m) % m;
		return children[i].size() == m;
	}

	bool firstFull() {
		return children[h].size() == m;
	}

	bool tooFull() {
		return children[h].size() > 0 && children[(h - 1 + m) % m].size() > 0;
	}

	bool tooEmpty() {
		return n < m * m / 8;
	}

public:
	DequeK2TieredVector() {
		init(DEFAULT_SIZE);
	}

	DequeK2TieredVector(int32_t size) {
		init(size);
	}

	~DequeK2TieredVector(void) {
		delete[] children;
	}

	int32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		int32_t l0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		int32_t newR = i == 0 ? r : (r - l0) % m;

		return children[elem(i)].getElemAt(newR);
	}

	void setElemAt(int32_t r, int32_t e) {
		int32_t l0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		int32_t newR = i == 0 ? r : (r - l0) % m;

		return children[elem(i)].setElemAt(newR, e);
	}

	void insertElemAt(int32_t r, int32_t e) {
#if DEBUG
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
#endif
		if (r == n) {
			return insertLast(e);
		}
		else if (r == 0) {
			return insertFirst(e);
		}

		bool insertFront = r < n - r;

		if (tooFull() && ((insertFront && firstFull()) || (!insertFront && lastFull()))) {
			doubleSize();
		}

		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) % m;

		if (insertFront) {
			if (children[h].isFull()) {
				incH(-1);
				i++;
			}
			if (i > 0) {
				newR--;
				if (newR < 0) {
					i--;
					newR = i == 0 ? children[h].size() : m - 1;
				}
			}
			for (int32_t j = 0; j < i; j++) {
				children[elem(j)].insertLast(children[elem(j + 1)].removeFirst());
			}
		}
		else {
			int32_t back = (int32_t)ceil((double)(n - n0) / m);

			if (children[elem(back)].isFull()) {
				back++;
			}

			for (int32_t j = back; j > i; j--) {
				children[elem(j)].insertFirst(children[elem(j - 1)].removeLast());
			}
		}

		children[elem(i)].insertElemAt(newR, e);

		n++;
	}

	void insertFirst(int32_t e) {
		if (tooFull() && firstFull()) {
			doubleSize();
		}

		if (children[h].isFull())
			incH(-1);
		children[h].insertFirst(e);
		n++;
	}

	void insertLast(int32_t e) {
		if (tooFull() && lastFull()) {
			doubleSize();
		}

		int32_t i = n / m;
		if (children[elem(i)].isFull())
			i++;
		children[elem(i)].insertLast(e);
		n++;
	}

	int32_t removeElemAt(int32_t r) {
		if (tooEmpty()) {
			halveSize();
		}

		int32_t e;
		bool removeFront = r < n - r;

		int32_t l0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		int32_t newR = i == 0 ? r : (r - l0) % m;

		if (removeFront) {
			e = children[elem(i)].removeElemAt(newR);

			for (int32_t j = i; j > 0; j--) {
				children[elem(j)].insertFirst(children[elem(j - 1)].removeLast());
			}

			if (children[h].isEmpty()) {
				incH(1);
			}

		}
		else {
			e = children[elem(i)].removeElemAt(newR);

			int32_t back = (int32_t)ceil((double)(n - l0) / m);

			if (children[elem(back)].isEmpty()) {
				back--;
			}

			for (int32_t j = i; j < back; j++) {
				children[elem(j)].insertLast(children[elem(j + 1)].removeFirst());
			}
		}

		n--;
		return e;
	}

	int32_t removeFirst() {
		return removeElemAt(0);
	}

	int32_t removeLast() {
		return removeElemAt(n);
	}

	bool isFull() {
		return n == m * m;
	}

	bool isEmpty() {
		return n == 0;
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += children[h].toStringPretty();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[elem(i)].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += children[0].toString();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + children[i].toString();
		}
		return s += " }";
	}
};