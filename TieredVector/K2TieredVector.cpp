#include "stdafx.h"
#include "Deque.cpp"

class K2TieredVector : public RankSequence {
protected:
	Deque *subvectors;

	void init(int32_t size) {
		this->m = size;
		subvectors = new Deque[size];
	}

	void doubleSize() {
		int32_t oldM = m;
		m *= 2;

		Deque *newSubvectors = new Deque[m];
		for (int32_t i = 0; i < oldM; i++) {
			newSubvectors[i] = subvectors[i];
			newSubvectors[i].doubleSize();
		}
		for (int32_t i = oldM; i < m; i++) {
			delete[](newSubvectors[i].a);
			newSubvectors[i] = Deque(m);
		}
		delete[] subvectors;
		subvectors = newSubvectors;

		// Move everything to the lower 1/4 of the current TV
		int32_t nextDequeWithElems = 1;
		int32_t moved = 0;
		int32_t limit = n - subvectors[0].n;
		for (int32_t i = 0; moved < limit; i++) {
			while (!subvectors[i].isFull()) {
				subvectors[i].insertLast(subvectors[nextDequeWithElems].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (subvectors[nextDequeWithElems].isEmpty()) {
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
			newA[i] = subvectors[i / 2].lowerHalf();
			i++;
			newA[i] = subvectors[i / 2].upperHalf();
		}
		delete[] subvectors;
		subvectors = newA;

		m /= 2;
	}

public:
	int32_t m = 0;	// Number of subvectors. This is the same as the capacity of each subvector since we only have k = 2

	K2TieredVector(int32_t size) {
		init(size);
		for (int32_t i = 0; i < size; i++) {
			delete[] (subvectors[i].a);
			subvectors[i] = Deque(size);
		}
	}

	K2TieredVector(void) {
		init(DEFAULT_SIZE);
	}

	~K2TieredVector(void) {
		for (int32_t i = 0; i < m; i++) {
			delete[] (subvectors[i].a);
		}
		delete[] subvectors;
	}

	int32_t getElemAt(int32_t r) {
		int32_t i = r / m;
		return subvectors[i].getElemAt(r - i*m);
	}

	void insertElemAt(int32_t r, int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = r / m;

		if (subvectors[i].isFull()) {
			int32_t back = n / m;
			if (subvectors[back].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				subvectors[j].insertFirst(subvectors[j - 1].removeLast());
			}
		}
		subvectors[i].insertElemAt(r - i*m, e);
		n++;
	}

	void insertLast(int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = n / m;
		if (subvectors[i].isFull())
			i++;
		subvectors[i].insertLast(e);
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
		int32_t e = subvectors[i].removeElemAt(r - i*m);
		for (int32_t j = i; j < (n-1) / m; j++) {
			subvectors[j].insertLast(subvectors[j + 1].removeFirst());
		}
		n--;
		return e;
	}

	int32_t removeLast() {
		if (n < m * m / 8) {
			halveSize();
		}

		int32_t i = (n - 1) / m;
		int32_t e = subvectors[i].removeElemAt(n - 1 - i*m);
		n--;
		return e;
	}

	bool isFull() {
		return subvectors[m - 1].isFull();
	}

	string toStringSimple() {
		string s = "";
		if (m > 0) {
			s += subvectors[0].toStringSimple();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + subvectors[i].toStringSimple();
		}
		return s;
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += subvectors[0].toStringPretty();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + subvectors[i].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += subvectors[0].toString();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + subvectors[i].toString();
		}
		return s += " }";
	}
};


class BitTrickK2TieredVector : public RankSequence {
protected:
	BitTrickDeque *subvectors;

	int8_t shift;

	void doubleSize() {
		int32_t oldM = m;
		m = m << 1;

		BitTrickDeque *newSubvectors = new BitTrickDeque[m];
		for (int32_t i = 0; i < oldM; i++) {
			newSubvectors[i] = subvectors[i];
			newSubvectors[i].doubleSize();
		}
		for (int32_t i = oldM; i < m; i++) {
			delete[](newSubvectors[i].a);
			newSubvectors[i] = BitTrickDeque(m);
		}
		delete[] subvectors;
		subvectors = newSubvectors;
		
		// Move everything to the lower 1/4 of the current TV
		int32_t nextDequeWithElems = 1;
		int32_t moved = 0;
		int32_t limit = n - subvectors[0].n;
		for (int32_t i = 0; moved < limit; i++) {
			while (!subvectors[i].isFull()) {
				subvectors[i].insertLast(subvectors[nextDequeWithElems].removeFirst());
				moved++;
				if (moved >= limit) {
					break;
				}

				while (subvectors[nextDequeWithElems].isEmpty()) {
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
			newSubvectors[i] = subvectors[i >> 1].lowerHalf();
			i++;
			newSubvectors[i] = subvectors[i >> 1].upperHalf();
		}
		delete[] subvectors;
		subvectors = newSubvectors;

		shift--;
	}

public:
	int32_t m = 0;

	BitTrickK2TieredVector() {
		this->m = DEFAULT_SIZE;
		this->shift = (int32_t)log2(DEFAULT_SIZE);
		this->subvectors = new BitTrickDeque[DEFAULT_SIZE];
	}

	~BitTrickK2TieredVector(void) {
		for (int32_t i = 0; i < m; i++) {
			delete[](subvectors[i].a);
		}
		delete[] subvectors;
	}

	int32_t getElemAt(int32_t r) {
		int32_t i = r >> shift;
		return subvectors[i].getElemAt(r - (i << shift));
	}

	void insertElemAt(int32_t r, int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = r >> shift;

		if (subvectors[i].isFull()) {
			int32_t back = n >> shift;
			if (subvectors[back].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				subvectors[j].insertFirst(subvectors[j - 1].removeLast());
			}
		}
		subvectors[i].insertElemAt(r - (i << shift), e);
		n++;
	}

	void insertLast(int32_t e) {
		if (isFull()) {
			doubleSize();
		}

		int32_t i = n >> shift;
		if (subvectors[i].isFull())
			i++;
		subvectors[i].insertLast(e);
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
		int32_t e = subvectors[i].removeElemAt(r - (i << shift));
		for (int32_t j = i; j < (n - 1) >> shift; j++) {
			subvectors[j].insertLast(subvectors[j + 1].removeFirst());
		}
		n--;
		return e;
	}

	int32_t removeLast() {
		if (n < m << shift >> 3) {		// Divide by 8
			halveSize();
		}

		int32_t i = (n - 1) >> shift;
		int32_t e = subvectors[i].removeElemAt(n - 1 - (i << shift));
		n--;
		return e;
	}

	bool isFull() {
		return subvectors[m - 1].isFull();
	}

	string toStringSimple() {
		string s = "";
		if (m > 0) {
			s += subvectors[0].toStringSimple();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + subvectors[i].toStringSimple();
		}
		return s;
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += subvectors[0].toStringPretty();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + subvectors[i].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += subvectors[0].toString();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + subvectors[i].toString();
		}
		return s += " }";
	}
};

// TODO: Make a version that utilises bitshifts
class DequeK2TieredVector : public RankSequence {
private:
	int32_t h;
	int32_t size;		// Amount of subvectors with elements. Maximum m

	Deque *cd;

	int32_t elem(int32_t r) {
		return (h + r) % m;
	}

	void incH(int8_t inc) {
		h = (h + inc + m) % m;
	}

	void init(int32_t size) {
		this->h = 0;
		this->m = size;
		cd = new Deque[size];
	}

	void doubleSize() {
		Deque *newA = new Deque[m * 2];
		for (int32_t i = 0; i < m * 2; i++) {
			delete[](newA[i].a);
			newA[i] = Deque(m * 2);
		}

		// Move everything to the lower 1/4 of the current TV
		int32_t limit = n;
		int32_t moved = 0;
		int32_t cdToRemoveFrom = 0;
		Deque oldCd = cd[elem(cdToRemoveFrom)];
		for (int32_t i = 0; moved < limit; i++) {
			while (!newA[i].isFull() && moved < limit) {
				newA[i].insertLast(oldCd.removeFirst());
				moved++;
				if (oldCd.isEmpty()) {
					cdToRemoveFrom++;
					oldCd = cd[elem(cdToRemoveFrom)];
				}
			}
		}
		delete[] cd;
		cd = newA;

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

		delete[] cd;
		cd = newA;

		m /= 2;
		h = 0;
	}

public:
	int32_t m = 0;		// Number of subvectors

	DequeK2TieredVector() {
		init(DEFAULT_SIZE);
	}

	DequeK2TieredVector(int32_t size) {
		init(size);
	}

	~DequeK2TieredVector(void) {
		delete[] cd;
	}

	int32_t getElemAt(int32_t r) {
		int32_t l0 = cd[h].n;
		int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		int32_t newR = i == 0 ? r : (r - l0) % m;

		return cd[elem(i)].getElemAt(newR);
	}

	void insertElemAt(int32_t r, int32_t e) {
#if DEBUG
		cout << "Inserting " << e << " at " << r << endl;
		//cout << toStringPretty() << endl;
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
#endif
		if (r == n) {
			return insertLast(e);
		}

		int32_t l0 = cd[h].n;
		int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		int32_t newR = i == 0 ? r : (r - l0) % m;

		bool insertFront = r < n - r;

		if (isFull() && ((insertFront && firstFull()) || (!insertFront && lastFull()))) {
			doubleSize();
			int32_t l0 = cd[h].n;
			i = (int32_t)ceil((double)(r + 1 - l0) / m);
			newR = i == 0 ? r : (r - l0) % m;
		}

		if (insertFront) {
			if (cd[h].isFull()) {
				incH(-1);
				i++;
			}
			if (i > 0) {
				newR--;
				if (newR < 0) {
					i--;
					newR = i == 0 ? cd[h].n : m - 1;
				}
			}
			for (int32_t j = 0; j < i; j++) {
				cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
			}
		}
		else {
			int32_t back = (int32_t)ceil((double)(n - l0) / m);


			if (cd[elem(back)].isFull()) {
				back++;
			}
			for (int32_t j = back; j > i; j--) {
				cd[elem(j)].insertFirst(cd[elem(j - 1)].removeLast());
			}
		}

		cd[elem(i)].insertElemAt(newR, e);

		n++;
	}

	void insertFirst(int32_t e) {
		insertElemAt(0, e);
	}

	void insertLast(int32_t e) {
		if (isFull() && lastFull()) {
			doubleSize();
		}

		int32_t i = n / m;
		if (cd[elem(i)].isFull())
			i++;
		cd[elem(i)].insertLast(e);
		n++;
	}

	int32_t removeElemAt(int32_t r) {
#if DEBUG
		cout << "Removing at " << r << " in DequeK2TieredVector. n: " << n << endl;
#endif
		if (tooEmpty()) {
			halveSize();
		}

		int32_t e;
		bool removeFront = r < n - r;

		int32_t l0 = cd[h].n;
		int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		int32_t newR = i == 0 ? r : (r - l0) % m;

		if (removeFront) {
			e = cd[elem(i)].removeElemAt(newR);

			for (int32_t j = i; j > 0; j--) {
				cd[elem(j)].insertFirst(cd[elem(j - 1)].removeLast());
			}

			if (cd[h].isEmpty()) {
				incH(1);
			}

		}
		else {
			e = cd[elem(i)].removeElemAt(newR);

			int32_t back = (int32_t)ceil((double)(n - l0) / m);

			if (cd[elem(back)].isEmpty()) {
				back--;
			}

			for (int32_t j = i; j < back; j++) {
				cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
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
		return cd[h].n > 0 && cd[(h - 1 + m) % m].n > 0;
	}

	bool isEmpty() {
		return n == 0;
	}

	bool lastFull() {
		int32_t i = (h - 1 + m) % m;
		return cd[i].n == m;
	}

	bool firstFull() {
		return cd[h].n == m;
	}

	bool tooEmpty() {
		return n < m * m / 8;
	}

	string toStringPretty() {
		string s = "{ ";
		if (m > 0) {
			s += cd[h].toStringPretty();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + cd[elem(i)].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += cd[0].toString();
		}
		for (int32_t i = 1; i < m; i++) {
			s += ", " + cd[i].toString();
		}
		return s += " }";
	}
};