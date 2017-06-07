#include "stdafx.h"
//
//#ifndef _UTIL_INCLUDED
//#define _UTIL_INCLUDED
//#include "Util.h"
//#endif // ! _UTIL_INCLUDED
//
////#include "K2TieredVector.cpp"
//
//class TieredVector : public RankSequence {
//private:
//	int32_t h = 0;
//	int32_t t = 0;
//	int32_t h2 = -1;
//	int32_t t2 = -1;
//	int32_t n2 = 0;
//	int32_t k;
//
//	bool twoDeques = false;
//
//	TieredVector* subvectors;
//	int32_t* deque;
//
//	int32_t elem(int32_t r) {
//		return (h + r) % m;
//	}
//
//	int32_t elem2(int32_t r) {
//		return (h2 + r) % m;
//	}
//
//	void incH(int32_t inc) {
//		h = (h + inc + m) % m;
//	}
//
//	void incH2(int32_t inc) {
//		h2 = (h2 + inc + m) % m;
//	}
//
//	void init(int32_t k, int32_t size) {
//		this->m = size;
//		this->k = k;
//		if (k < 1) {
//			throw invalid_argument("k must be at least 1");
//		}
//		else if (k == 1) {
//			deque = new int32_t[size];
//		}
//		else {
//			subvectors = (TieredVector*)calloc(size, sizeof this);
//			for (int32_t i = 0; i < size; i++) {
//				TieredVector* newTv = new TieredVector(k - 1, size);
//				subvectors[i] = *newTv;
//				cout << "wattat" << endl;
//			}
//		}
//		cout << "Inited new tiered vector of k: " << k << endl;
//	}
//
//	void doubleSize() {
//		if (k == 1) {
//			int32_t *newDeque = new int32_t[m * 2];
//			for (int32_t i = 0; i < n; i++) {
//				newDeque[i] = getElemAt(i);
//			}
//			delete[] deque;
//			this->deque = newDeque;
//		}
//		else {
//			TieredVector* newSubvectors = (TieredVector*)calloc(m * 2, sizeof this);
//			for (int32_t i = 0; i < m * 2; i++) {
//				newSubvectors[i] = TieredVector(k - 1, m * 2);
//			}
//
//			// Move everything to the lower 1/4 of the current TV
//			int32_t limit = n;
//			int32_t moved = 0;
//			int32_t subvectorToRemoveFrom = 0;
//			TieredVector oldTv = subvectors[elem(subvectorToRemoveFrom)];
//			for (int32_t i = 0; moved < limit; i++) {
//				while (!newSubvectors[i].isFull() && moved < limit) {
//					newSubvectors[i].insertLast(oldTv.removeFirst());
//					moved++;
//					if (oldTv.isEmpty()) {
//						subvectorToRemoveFrom++;
//						oldTv = subvectors[elem(subvectorToRemoveFrom)];
//					}
//				}
//			}
//			delete[] subvectors;
//			subvectors = newSubvectors;
//		}
//
//		m *= 2;
//		h = 0;
//	}
//
//	void halveSize() {
//		if (k == 1) {
//			int32_t *newDeque = new int32_t[m / 2];
//			for (int32_t i = 0; i < m / 2; i++) {
//				newDeque[i] = getElemAt(i);
//			}
//			delete[] deque;
//			this->deque = newDeque;
//		}
//		else {
//			TieredVector *newSubvectors = (TieredVector*)calloc(m / 2, sizeof this);
//			for (int32_t i = 0; i < m / 2; i++) {
//				newSubvectors[i] = TieredVector(2, m / 2);
//			}
//
//			int32_t to = 0;
//			for (int32_t i = 0; i < n; i++) {
//				newSubvectors[to].insertElemAt(i % (m / 2), getElemAt(i));
//				if (newSubvectors[to].isFull())
//					to++;
//			}
//
//			delete[] subvectors;
//			this->subvectors = newSubvectors;
//		}
//
//		m /= 2;
//		h = 0;
//	}
//
//	void insertElemAtSecond(int32_t r, int32_t e) {
//		if (isFull())
//			throw exception("insertElemAtSecond should not be called if the tiered vector is full");
//		else if (k != 1)
//			throw exception("insertElemAtSecond should only be called on deques");
//		else if (!twoDeques) {
//			h2 = h;
//			incH2(-1);
//			twoDeques = true;
//		}
//		for (int32_t i = h2 + n2; i > h2 + r; i--) {
//			deque[(i + m) % m] = deque[(i + m - 1) % m];
//		}
//
//		deque[(h2 + r) % m] = e;
//		n2++;
//	}
//
//	int32_t removeElementFromSecond(int32_t r) {
//		if (k != 1)
//			throw exception("insertElemAtSecond should only be called on deques");
//		else if (!twoDeques) {
//			h2 = h;
//			incH2(-1);
//			twoDeques = true;
//		}
//		for (int32_t i = h2 + n2; i > h2 + r; i--) {
//			deque[(i + m) % m] = deque[(i + m - 1) % m];
//		}
//
//		deque[(h2 + r) % m] = e;
//		n2++;
//	}
//
//public:
//	int32_t n = 0;		// Number of elements
//	int32_t m = 0;		// Number of subvectors
//
//	TieredVector(int32_t k) {
//		init(k, DEFAULT_SIZE);
//	}
//
//	TieredVector(int32_t k, int32_t size) {
//		init(k, size);
//	}
//
//	~TieredVector(void) {
//		if (k == 1) {
//			delete[] deque;
//		}
//		else {
//			for (int32_t i = 0; i < m; i++) {
//				delete(&subvectors[i]);
//			}
//			delete[] subvectors;
//		}
//	}
//
//	int32_t getElemAt(int32_t r) {
//		// HUGE TODO
//		if (k == 1) {
//			return deque[elem(r)];
//		}
//		else {
//			int32_t l0 = subvectors[h].n;
//			int32_t i = ceil((double)(r + 1 - l0) / m);
//			int32_t newR = i == 0 ? r : (r - l0) % m;
//			return subvectors[elem(i)].getElemAt(newR);
//		}
//	}
//
//	// HUGE TODO
//	void insertElemAt(int32_t r, int32_t e) {
//#if DEBUG
//		cout << "Inserting " << e << " at " << r << ". k: " << k << endl;
//		//cout << toStringPretty() << endl;
//		//cout << toString() << endl;
//		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
//#endif
//		if (isFull()) {
//			doubleSize();
//		}
//
//		bool insertFront = r < n - r;
//
//		if (k == 1) {
//			if (insertFront) {
//				for (int32_t i = h + n; i > h + r; i--) {
//					deque[(i + m) % m] = deque[(i + m - 1) % m];
//				}
//			}
//			else {
//				for (int32_t i = h; i < h + r; i++) {
//					deque[(i + m - 1) % m] = deque[i % m];
//				}
//				incH(-1);
//			}
//			deque[(h + r) % m] = e;
//		}
//		else {
//			int32_t n0 = subvectors[h].n;
//			int32_t i = ceil((double)(r + 1 - n0) / m);
//			int32_t newR = i == 0 ? r : (r - n0) % m;
//
//			if (insertFront) {
//#if DEBUG
//				cout << "inserting from front" << endl;
//#endif
//				if (subvectors[h].isFull()) {
//					incH(-1);
//					i++;
//				}
//				if (i > 0) {
//					newR--;
//					if (newR < 0) {
//						i--;
//						newR = i == 0 ? subvectors[h].n : m - 1;
//					}
//					if (n == 0)
//						incH(1);
//				}
//				for (int32_t j = 0; j < i; j++) {
//					//cout << "i: " << i << " j: " << j << " elem(j): " << elem(j) << " elem(j + 1): " << elem(j+1) << endl;
//					subvectors[elem(j)].insertLast(subvectors[elem(j + 1)].removeFirst());
//				}
//			}
//			else {
//#if DEBUG
//				cout << "inserting from back" << endl;
//#endif
//
//				int32_t back = ceil((double)(n - n0) / m);
//
//				if (subvectors[elem(back)].isFull()) {
//					back++;
//				}
//				for (int32_t j = back; j > i; j--) {
//					if (j == back && elem(back) == h) {
//						subvectors[h].insertElemAtSecond(0, subvectors[elem(back - 1)].removeLast());
//					}
//					else {
//						subvectors[elem(j)].insertFirst(subvectors[elem(j - 1)].removeLast());
//					}
//				}
//			}
//			cout << elem(i) << endl;
//			subvectors[elem(i)].insertElemAt(newR, e);
//		}
//
//		n++;
//	}
//
//	void insertFirst(int32_t e) {
//		insertElemAt(0, e);
//	}
//
//	void insertLast(int32_t e) {
//		insertElemAt(n, e);
//	}
//	
//	int32_t removeElemAt(int32_t r) {
//		throw exception("removal from general tiered vector not implemented yet");
//		// TODO LATER
//		/*cout << "Removing at " << r << ". k: " << k << endl;
//		cout <<  toStringPretty() << endl;
//		//cout << toString() << endl;
//		checkIndexOutOfBounds(r, n, "remove", "TieredVector");
//		if (tooEmpty()) {
//			halveSize();
//		}
//
//		int32_t e;
//		bool removeFront = r < n - r;
//
//		if (k == 2) {
//			int32_t n0 = subDeques[h].n;
//			int32_t i = ceil((double)(r + 1 - n0) / l);
//			int32_t newR = i == 0 ? r : (r - n0) % l;
//
//			if (removeFront) {
//				e = subDeques[elem(i)].removeElemAt(newR);
//
//				for (int32_t j = i; j > 0; j--) {
//					subDeques[elem(j)].insertFirst(subDeques[elem(j - 1)].removeLast());
//				}
//
//				if (subDeques[h].isEmpty()) {
//					incH(1);
//				}
//
//			}
//			else {
//				e = subDeques[elem(i)].removeElemAt(newR);
//
//				int32_t back = ceil((double)(n - n0) / l);
//
//				if (subDeques[elem(back)].isEmpty()) {
//					back--;
//				}
//
//				for (int32_t j = i; j < back; j++) {
//					subDeques[elem(j)].insertLast(subDeques[elem(j + 1)].removeFirst());
//				}
//			}
//		}
//		else {
//			int32_t n0 = subvectors[h].n;
//			int32_t i = ceil((double)(r + 1 - n0) / l);
//			int32_t newR = i == 0 ? r : (r - n0) % l;
//
//			if (removeFront) {
//				e = subvectors[elem(i)].removeElemAt(newR);
//
//				for (int32_t j = i; j > 0; j--) {
//					subvectors[elem(j)].insertFirst(subvectors[elem(j - 1)].removeLast());
//				}
//
//				if (subvectors[h].isEmpty()) {
//					incH(1);
//				}
//			}
//			else {
//				e = subvectors[elem(i)].removeElemAt(newR);
//
//				int32_t back = ceil((double)(n - n0) / l);
//
//				if (subvectors[elem(back)].isEmpty()) {
//					back--;
//				}
//
//				for (int32_t j = i; j < back; j++) {
//					subvectors[elem(j)].insertLast(subvectors[elem(j + 1)].removeFirst());
//				}
//			}
//		}
//
//		n--;
//		return e;*/
//	}
//
//	int32_t removeFirst() {
//		return removeElemAt(0);
//	}
//
//	int32_t32_t removeLast() {
//		return removeElemAt(n - 1);
//	}
//
//	bool isFull() {
//		return n == (int32_t)pow(m, k);
//	}
//
//	bool isEmpty() {
//		return n == 0;
//	}
//
//	bool tooEmpty() {
//		return n < pow(m, k) / 8;
//	}
//
//	string toStringSimple() {
//		string s = "";
//		if (k == 1) {
//			if (m > 0) {
//				s += deque[h];
//			}
//			for (int32_t i = 1; i < m; i++) {
//				s += ", " + deque[elem(i)];
//			}
//		}
//		else {
//			if (m > 0) {
//				s += subvectors[h].toStringSimple();
//			}
//			for (int32_t i = 1; i < m; i++) {
//				s += ", " + subvectors[elem(i)].toStringSimple();
//			}
//		}
//		return s;
//	}
//
//	string toStringPretty() {
//		string s = "{ ";
//		if (k == 1) {
//			if (m > 0) {
//				s += deque[h];
//			}
//			for (int32_t i = 1; i < m; i++) {
//				s += ", " + deque[elem(i)];
//			}
//		}
//		else {
//			if (m > 0) {
//				s += subvectors[h].toStringPretty();
//			}
//			for (int32_t i = 1; i < m; i++) {
//				s += ", " + subvectors[elem(i)].toStringPretty();
//			}
//		}
//		return s += " }";
//	}
//
//	string toString() {
//		string s = "{ ";
//		if (k == 1) {
//			if (m > 0) {
//				s += deque[0];
//			}
//			for (int32_t i = 1; i < m; i++) {
//				s += ", " + deque[i];
//			}
//		}
//		else {
//			if (m > 0) {
//				s += subvectors[0].toString();
//			}
//			for (int32_t i = 1; i < m; i++) {
//				s += ", " + subvectors[i].toString();
//			}
//		}
//		return s += " }";
//	}
//};