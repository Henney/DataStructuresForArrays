#include "stdafx.h"
#include "Deque.cpp"

class Simple2TieredVector : public ArrayDataStructure {
private:
	int32_t n = 0;
	int32_t m = 0;	// Number of children
protected:
	vector<Deque> children;

	void init(int32_t size) {
		this->m = size;
		this->children = vector<Deque>(m);
	}

	void doubleSize() {
		int32_t oldM = m;
		m *= 2;

		vector<Deque> newSubvectors(m);
		for (int32_t i = 0; i < oldM; i++) {
			newSubvectors[i] = children[i];
			newSubvectors[i].doubleSize();
		}
		for (int32_t i = oldM; i < m; i++) {
			newSubvectors[i] = Deque(m);
		}
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
		vector<Deque> newA(m / 2);

		for (int32_t i = 0; i < m / 2; i++) {
			newA[i] = Deque(m / 2);
		}

		for (int32_t i = 0; i < m / 4; i++) {
			newA[i] = children[i / 2].lowerHalf();
			i++;
			newA[i] = children[i / 2].upperHalf();
		}
		children = newA;

		m /= 2;
	}

public:
	Simple2TieredVector(int32_t size) {
		init(size);
		for (int32_t i = 0; i < size; i++) {
			children[i] = Deque(size);
		}
	}

	Simple2TieredVector(void) {
		init(DEFAULT_SIZE);
	}

	~Simple2TieredVector(void) {
		children.clear();
		vector<Deque>().swap(children);
	}

	uint32_t size() {
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


class BitTrickSimple2TieredVector : public ArrayDataStructure {
private:
	int32_t n = 0;
	int32_t m = 0;
protected:
	vector<BitTrickDeque> children;

	int8_t shift;

	void doubleSize() {
		int32_t oldM = m;
		m = m << 1;

		vector<BitTrickDeque> newSubvectors(m);
		for (int32_t i = 0; i < oldM; i++) {
			newSubvectors[i] = children[i];
			newSubvectors[i].doubleSize();
		}
		for (int32_t i = oldM; i < m; i++) {
			newSubvectors[i] = BitTrickDeque(m);
		}
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

		vector<BitTrickDeque> newSubvectors(m);

		for (int32_t i = 0; i < m; i++) {
			newSubvectors[i] = BitTrickDeque(m);
		}

		for (int32_t i = 0; i < m >> 1; i++) {
			newSubvectors[i] = children[i >> 1].lowerHalf();
			i++;
			newSubvectors[i] = children[i >> 1].upperHalf();
		}
		children = newSubvectors;

		shift--;
	}

public:
	BitTrickSimple2TieredVector() {
		this->m = DEFAULT_SIZE;
		this->shift = (int32_t)log2(DEFAULT_SIZE);
		this->children = vector<BitTrickDeque>(DEFAULT_SIZE);
	}

	~BitTrickSimple2TieredVector(void) {
		children.clear();
		vector<BitTrickDeque>().swap(children);
	}

	uint32_t size() {
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

class Deque2TieredVector : public ArrayDataStructure {
private:
	int32_t n = 0;
	int32_t h = 0;
	int32_t m = 0;		// Number of children

	vector<Deque> children;

	int32_t child(int32_t r) {
		return (h + r) % m;
	}

	void incH(int8_t inc) {
		h = (h + inc + m) % m;
	}

	void init(int32_t size) {
		this->m = size;
		this->children = vector<Deque>(size);
	}

	void doubleSize() {
		vector<Deque> newA(m * 2);
		for (int32_t i = 0; i < m * 2; i++) {
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
		children = newA;

		m *= 2;
		h = 0;
	}

	void halveSize() {
		vector<Deque> newA(m / 2);

		for (int32_t i = 0; i < m / 2; i++) {
			newA[i] = Deque(m / 2);
		}

		int32_t to = 0;
		for (int32_t i = 0; i < n; i++) {
			newA[to].insertElemAt(i % (m / 2), getElemAt(i));
			if (newA[to].isFull())
				to++;
		}

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
	Deque2TieredVector() {
		init(DEFAULT_SIZE);
	}

	Deque2TieredVector(int32_t size) {
		init(size);
	}

	~Deque2TieredVector(void) {
		children.clear();
		vector<Deque>().swap(children);
	}

	uint32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) % m;

		return children[child(i)].getElemAt(newR);
	}

	void setElemAt(int32_t r, int32_t e) {
		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) % m;

		return children[child(i)].setElemAt(newR, e);
	}

	void insertElemAt(int32_t r, int32_t e) {
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
				children[child(j)].insertLast(children[child(j + 1)].removeFirst());
			}
		}
		else {
			int32_t back = (int32_t)ceil((double)(n - n0) / m);

			if (children[child(back)].isFull()) {
				back++;
			}

			for (int32_t j = back; j > i; j--) {
				children[child(j)].insertFirst(children[child(j - 1)].removeLast());
			}
		}

		children[child(i)].insertElemAt(newR, e);

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
		if (children[child(i)].isFull())
			i++;
		children[child(i)].insertLast(e);
		n++;
	}

	int32_t removeElemAt(int32_t r) {
		if (tooEmpty()) {
			halveSize();
		}

		int32_t e;
		bool removeFront = r < n - r;

		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) % m;

		if (removeFront) {
			e = children[child(i)].removeElemAt(newR);

			for (int32_t j = i; j > 0; j--) {
				children[child(j)].insertFirst(children[child(j - 1)].removeLast());
			}

			if (children[h].isEmpty()) {
				incH(1);
			}

		}
		else {
			e = children[child(i)].removeElemAt(newR);

			int32_t back = (int32_t)ceil((double)(n - n0) / m);

			if (children[child(back)].isEmpty()) {
				back--;
			}

			for (int32_t j = i; j < back; j++) {
				children[child(j)].insertLast(children[child(j + 1)].removeFirst());
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
			s += ", " + children[child(i)].toStringPretty();
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

// TODO
class BitTrickDeque2TieredVector : public ArrayDataStructure {
private:
	int32_t n = 0;
	int32_t h = 0;
	int32_t m = 0;		// Number of children

	int8_t shift;

	vector<BitTrickDeque> children;

	int32_t child(int32_t r) {
		return (h + r) & (m - 1);
	}

	void incH(int8_t inc) {
		h = (h + inc + m) & (m - 1);
	}

	void doubleSize() {
		int32_t newM = m << 1;
		vector<BitTrickDeque> newA(newM);
		for (int32_t i = 0; i < newM; i++) {
			newA[i] = BitTrickDeque(newM);
		}

		// Move everything to the lower 1/4 of this tiered vector
		int32_t idx = 0;
		for (int32_t i = 0; i < m; i++) {
			while (!newA[i].isFull() && idx < n) {
				newA[i].insertLast(getElemAt(idx));
				idx++;
			}
		}
		children = newA;

		m = newM;
		shift++;
		h = 0;
	}

	void halveSize() {
		int32_t newM = m >> 1;
		vector<BitTrickDeque> newA(newM);

		for (int32_t i = 0; i < newM; i++) {
			newA[i] = BitTrickDeque(newM);
		}

		int32_t to = 0;
		for (int32_t i = 0; i < n; i++) {
			newA[to].insertElemAt(i & (newM - 1), getElemAt(i));
			if (newA[to].isFull())
				to++;
		}

		children = newA;

		m = newM;
		shift--;
		h = 0;
	}

	bool lastFull() {
		int32_t i = (h - 1 + m) & (m - 1);
		return children[i].size() == m;
	}

	bool firstFull() {
		return children[h].size() == m;
	}

	bool tooFull() {
		return children[h].size() > 0 && children[(h - 1 + m) & (m - 1)].size() > 0;
	}

	bool tooEmpty() {
		return n < m << shift >> 3;
	}

public:
	BitTrickDeque2TieredVector() {
		this->m = DEFAULT_SIZE;
		this->children = vector<BitTrickDeque>(DEFAULT_SIZE);
		this->shift = (int32_t)log2(DEFAULT_SIZE);
	}

	~BitTrickDeque2TieredVector(void) {
		children.clear();
		vector<BitTrickDeque>().swap(children);
	}

	uint32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) & (m - 1);

		return children[child(i)].getElemAt(newR);
	}

	void setElemAt(int32_t r, int32_t e) {
		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) & (m - 1);

		return children[child(i)].setElemAt(newR, e);
	}

	void insertElemAt(int32_t r, int32_t e) {
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
		int32_t newR = i == 0 ? r : (r - n0) & (m - 1);

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
				children[child(j)].insertLast(children[child(j + 1)].removeFirst());
			}
		}
		else {
			int32_t back = (int32_t)ceil((double)(n - n0) / m);

			if (children[child(back)].isFull()) {
				back++;
			}

			for (int32_t j = back; j > i; j--) {
				children[child(j)].insertFirst(children[child(j - 1)].removeLast());
			}
		}

		children[child(i)].insertElemAt(newR, e);

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

		int32_t i = n >> shift;
		if (children[child(i)].isFull())
			i++;
		children[child(i)].insertLast(e);
		n++;
	}

	int32_t removeElemAt(int32_t r) {
		if (tooEmpty()) {
			halveSize();
		}

		int32_t e;
		bool removeFront = r < n - r;

		int32_t n0 = children[h].size();
		int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		int32_t newR = i == 0 ? r : (r - n0) & (m - 1);

		if (removeFront) {
			e = children[child(i)].removeElemAt(newR);

			for (int32_t j = i; j > 0; j--) {
				children[child(j)].insertFirst(children[child(j - 1)].removeLast());
			}

			if (children[h].isEmpty()) {
				incH(1);
			}

		}
		else {
			e = children[child(i)].removeElemAt(newR);

			int32_t back = (int32_t)ceil((double)(n - n0) / m);

			if (children[child(back)].isEmpty()) {
				back--;
			}

			for (int32_t j = i; j < back; j++) {
				children[child(j)].insertLast(children[child(j + 1)].removeFirst());
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
		return n == m << shift;
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
			s += ", " + children[child(i)].toStringPretty();
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


class UnamortisedBitTrickSimple2TieredVector : public ArrayDataStructure {
private:
	int32_t n = 0;
	int32_t m = 0;

	int32_t moreN = 0;
	int32_t fewerN = 0;
protected:
	vector<BitTrickDeque> fewerChildren;
	vector<BitTrickDeque> children;
	vector<BitTrickDeque> moreChildren;

	int8_t shift;

	void doubleSize() {
		int32_t oldM = m;
		m = m << 1;		
		children.swap(moreChildren);
		moreChildren.clear();
		moreChildren = vector<BitTrickDeque>(m << 1);
		for (int32_t i = 0; i < m << 1; i++) {
			moreChildren[i] = BitTrickDeque(m << 1);
		}
		moreN = 0;

		fewerChildren.clear();
		fewerChildren = vector<BitTrickDeque>(oldM);
		for (int32_t i = 0; i < oldM; i++) {
			fewerChildren[i] = BitTrickDeque(oldM);
		}
		shift++;
	}

	void halveSize() {
		int32_t oldM = m;
		m = m >> 1;

		children.swap(fewerChildren);
		fewerChildren = vector<BitTrickDeque>(m >> 1);
		for (int32_t i = 0; i < m >> 1; i++) {
			fewerChildren[i] = BitTrickDeque(m >> 1);
		}
		fewerN = 0;

		moreChildren.clear();
		moreChildren = vector<BitTrickDeque>(oldM);
		for (int32_t i = 0; i < oldM; i++) {
			moreChildren[i] = BitTrickDeque(oldM);
		}

		shift--;
	}

public:
	UnamortisedBitTrickSimple2TieredVector() {
		this->m = DEFAULT_SIZE;
		this->shift = (int32_t)log2(DEFAULT_SIZE);
		this->children = vector<BitTrickDeque>(DEFAULT_SIZE);
		this->fewerChildren = vector<BitTrickDeque>(DEFAULT_SIZE >> 1);
		this->moreChildren = vector<BitTrickDeque>(DEFAULT_SIZE << 1);
		for (int32_t i = 0; i < m << 1; i++) {
			moreChildren[i] = BitTrickDeque(m << 1);
		}
	}

	~UnamortisedBitTrickSimple2TieredVector(void) {
		children.clear();
		vector<BitTrickDeque>().swap(children);
		fewerChildren.clear();
		vector<BitTrickDeque>().swap(fewerChildren);
		moreChildren.clear();
		vector<BitTrickDeque>().swap(moreChildren);
	}

	uint32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		int32_t i = r >> shift;
		return children[i].getElemAt(r - (i << shift));
	}

	void setElemAt(int32_t r, int32_t e) {
		throw exception("Not implemented");
		int32_t i = r >> shift;
		children[i].setElemAt(r - (i << shift), e);
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

		if (r < moreN) {
			insertElemAtMore(r, e);
		}
		if (moreN < n) {
			insertMoreLast(getElemAt(moreN));
		}
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

	void insertElemAtMore(int32_t r, int32_t e) {
		int8_t moreShift = shift + 1;
		int32_t i = r >> moreShift;

		if (moreChildren[i].isFull()) {
			int32_t back = moreN >> moreShift;
			if (moreChildren[back].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				moreChildren[j].insertFirst(moreChildren[j - 1].removeLast());
			}
		}
		moreChildren[i].insertElemAt(r - (i << moreShift), e);
		moreN++;
	}

	void insertMoreLast(int32_t e) {
		insertElemAtMore(moreN, e);
	}

	void insertElemAtFewer(int32_t r, int32_t e) {
		int8_t fewerShift = shift - 1;
		int32_t i = r >> fewerShift;

		if (fewerChildren[i].isFull()) {
			int32_t back = fewerN >> fewerShift;
			if (fewerChildren[back].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				fewerChildren[j].insertFirst(fewerChildren[j - 1].removeLast());
			}
		}
		fewerChildren[i].insertElemAt(r - (i << fewerShift), e);
		fewerN++;
	}

	void insertFewerLast(int32_t e) {
		insertElemAtFewer(fewerN, e);
	}

	int32_t removeElemAt(int32_t r) {
		if (n < (m << shift) >> 3) {		// Divide by 8
			halveSize();
		}

		int32_t i = r >> shift;
		int32_t e = children[i].removeElemAt(r - (i << shift));
		for (int32_t j = i; j < (n - 1) >> shift; j++) {
			children[j].insertLast(children[j + 1].removeFirst());
		}

		n--;

		if (fewerN > r) {
			removeElemAtFewer(r);
		}
		if (moreN > r) {
			removeElemAtMore(r);
		}

		if (n <= m << shift >> 1 && fewerN < n) {
			insertFewerLast(getElemAt(fewerN));
		}

		return e;
	}

	int32_t removeLast() {
		if (n < m << shift >> 3) {		// Divide by 8
			halveSize();
		}

		int32_t i = (n - 1) >> shift;
		int32_t e = children[i].removeElemAt(n - 1 - (i << shift));
		n--;

		if (fewerN > n) {
			removeElemAtFewer(fewerN);
		}
		if (moreN > n) {
			removeElemAtMore(moreN);
		}

		if (n <= m << shift >> 1 && fewerN < n) {
			insertFewerLast(getElemAt(fewerN));
		}

		return e;
	}

	void removeElemAtMore(int32_t r) {
		int8_t moreShift = shift + 1;
		int32_t i = r >> moreShift;
		int32_t e = moreChildren[i].removeElemAt(r - (i << moreShift));
		for (int32_t j = i; j < (moreN - 1) >> moreShift; j++) {
			moreChildren[j].insertLast(moreChildren[j + 1].removeFirst());
		}
		moreN--;
	}

	void removeElemAtFewer(int32_t r) {
		int8_t fewerShift = shift - 1;
		int32_t i = r >> fewerShift;
		int32_t e = fewerChildren[i].removeElemAt(r - (i << fewerShift));
		for (int32_t j = i; j < (fewerN - 1) >> fewerShift; j++) {
			fewerChildren[j].insertLast(fewerChildren[j + 1].removeFirst());
		}
		fewerN--;
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
		s += " }";

		s += "\n{ ";
		if (m > 0) {
			s += moreChildren[0].toStringPretty();
		}
		for (int32_t i = 1; i < m << 1; i++) {
			s += ", " + moreChildren[i].toStringPretty();
		}
		
		s += " }";

		s += "\n{ ";
		if (m > 0) {
			s += fewerChildren[0].toStringPretty();
		}
		for (int32_t i = 1; i < m >> 1; i++) {
			s += ", " + fewerChildren[i].toStringPretty();
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