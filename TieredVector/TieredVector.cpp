#include "stdafx.h"

#include "Util.h"

// TODO: Make a version that utilises bitshifts
class TieredVector : public ArrayDataStructure {
private:
	int32_t k;
	int32_t m = 0;		// Number of children
	bool topLevel = false;

	int32_t n = 0;
	int32_t h = 0;

	int32_t endH = 0;
	int32_t endN = 0;

	vector<TieredVector> children;
	vector<int32_t> elements;

	int32_t child(int32_t r) {
		return (h + r) % m;
	}

	int32_t childEnd(int32_t r) {
		if (k > 1)
			throw exception("Can only call elem2 on a 1-tiered vector");
		return (endH + r) % m;
	}

	void incH(int8_t inc) {
		h = (h + inc + m) % m;
	}

	void incH2(int8_t inc) {
		if (k > 1)
			throw exception("Can only call incH2 on a 1-tiered vector");
		endH = (endH + inc + m) % m;
	}

	void init(int32_t k, int32_t size, bool topLevel) {
		this->k = k;
		this->m = size;
		this->topLevel = topLevel;

		if (k < 1) {
			throw invalid_argument("k must be a positive integer");
		}
		else if (k == 1) {
			elements = vector<int32_t>(m);
		}
		else {
			children;
			for (int32_t i = 0; i < m; i++) {
				children.insert(children.begin() + i, TieredVector(k - 1, m, false));
			}
		}
	}

	void doubleSize() {
		//Deque *newA = new Deque[m * 2];
		//for (int32_t i = 0; i < m * 2; i++) {
		//	delete[](newA[i].a);
		//	newA[i] = Deque(m * 2);
		//}

		//// Move everything to the lower 1/4 of this tiered vector
		//int32_t idx = 0;
		//for (int32_t i = 0; i < m; i++) {
		//	while (!newA[i].isFull() && idx < n) {
		//		newA[i].insertLast(getElemAt(idx));
		//		idx++;
		//	}
		//}
		//delete[] children;
		//children = newA;

		//m *= 2;
		//h = 0;
	}

	void halveSize() {
		//Deque *newA = new Deque[m / 2];

		//for (int32_t i = 0; i < m / 2; i++) {
		//	delete[](newA[i].a);
		//	newA[i] = Deque(m / 2);
		//}

		//int32_t to = 0;
		//for (int32_t i = 0; i < n; i++) {
		//	newA[to].insertElemAt(i % (m / 2), getElemAt(i));
		//	if (newA[to].isFull())
		//		to++;
		//}

		//delete[] children;
		//children = newA;

		//m /= 2;
		//h = 0;
	}

	bool tooEmpty() {
		return n < m * m / 8;
	}

	int32_t getElemAtEnd(int32_t r) {
		if (k > 1)
			throw exception("Can only call getElemAt2 on 1-tiered vector");
		return elements[childEnd(r)];
	}

	void insertElemAtEnd(int32_t r, int32_t e) {
		if (k > 1)
			throw exception("Can only call insertElemAt2 on 1-tiered vector");
		if (n + endN == m)
			throw exception("Cannot insert if the deque is already full");

		if (endN == 0) {
			endH = h;
			incH2(n);
		}
		for (int32_t i = endH + endN; i > endH + r; i--) {
			elements[(i + m) % m] = elements[(i + m - 1) % m];
		}
		elements[childEnd(r)] = e;
		endN++;
	}

	int32_t removeElemAtEnd(int32_t r) {
		if (k > 1)
			throw exception("Can only call removeElemAt2 on 1-tiered vector");

		if (topLevel && tooEmpty()) {
			halveSize();
		}

		int32_t e = elements[childEnd(r)];

		for (int32_t i = endH + r; i < endH + endN; i++) {
			elements[i % m] = elements[(i + m + 1) % m];
		}
		endN--;
		return e;
	}

	void transferElementsToEnd() {
		if (k > 1)
			throw exception("Can only call transferElementsToSecond on 1-tiered vector");
		if (endN != 0)
			throw exception("There must not be elements in the second part of the deque when transferring");
		endH = h;
		endN = n;
		n = 0;
	}

	void transferElementsToStart() {
		if (k > 1)
			throw exception("Can only call transferElementsToFirst on 1-tiered vector");
		if (n != 0)
			throw exception("There must not be elements in the first part of the deque when transferring");
		h = endH;
		n = endN;
		endN = 0;
	}

	TieredVector(int32_t k, bool topLevel) {
		init(k, DEFAULT_SIZE, topLevel);
	}

	TieredVector(int32_t k, int32_t size, bool topLevel) {
		init(k, size, topLevel);
	}

public:
	TieredVector(int32_t k) {
		init(k, DEFAULT_SIZE, true);
	}

	TieredVector(int32_t k, int32_t size) {
		init(k, size, true);
	}

	~TieredVector(void) {
		if (k == 1) {
			//delete[] elements;
		}
		else {
			//delete &children;
		}
	}

	uint32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		checkIndexOutOfBounds(r, n, "retrieve", "TieredVector");
		if (k == 1) {
			return elements[child(r)];
		}
		else {
			int32_t l = (int32_t)pow(m, k - 1);

			int32_t n0 = children[h].size();
			int32_t i = (int32_t)ceil((double)(r + 1 - n0) / l);
			int32_t newR = i == 0 ? r : (r - n0) % l;

			int32_t idx = child(i);

			if (k == 2 && idx == h && r >= l) {
				return children[idx].getElemAtEnd(newR);
			}
			else {
				return children[idx].getElemAt(newR);
			}
		}
	}

	void setElemAt(int32_t r, int32_t e) {
		throw exception("Not implemented");
		// TODO
		//if (k == 1) {
		//	elements[elem(r)] = e;
		//}
		//else {
		//	int32_t n0 = children[h].size();
		//	int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
		//	int32_t newR = i == 0 ? r : (r - n0) % m;

		//	children[elem(i)].setElemAt(newR, e);
		//}
	}

	void insertElemAt(int32_t r, int32_t e) {
#if DEBUG
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
		if (n == pow(m, k)) {
			throw exception("TieredVector is full");
		}
#endif

		if (isFull()) {
			doubleSize();
		}

		bool insertFront = r < n - r;

		if (k == 1) {

			if (insertFront || endN > 0) { // Always have to insert from the front if the deque has "two" deques
				for (int32_t i = h; i < h + r; i++) {
					elements[(i + m - 1) % m] = elements[i % m];
				}
				incH(-1);
			}
			else {
				for (int32_t i = h + n; i > h + r; i--) {
					elements[(i + m) % m] = elements[(i + m - 1) % m];
				}
			}
			elements[(h + r) % m] = e;
		}
		else {
			if (e == 22 && r == 23) {
				cout << endl;
			}
			int32_t l = (int32_t)pow(m, k - 1);

			int32_t n0 = children[h].size();
			int32_t i = (int32_t)ceil((double)(r + 1 - n0) / l);
			int32_t newR = i == 0 ? r : (r - n0) % l;

			if (insertFront) {
				if (children[h].isFull()) {
					incH(-1);
					i++;
					if (!children[h].isEmpty() && k == 2) {
						children[h].transferElementsToEnd();
					}
				}
				if (i > 0) {
					newR--;
					if (newR < 0) {
						i--;
						newR = i == 0 ? children[h].size() : l - 1;
					}
				}

				for (int32_t j = 0; j < i; j++) {
					children[child(j)].insertLast(children[child(j + 1)].removeFirst());
				}

				if (k == 2 && child(i) == h && r > n0 && children[h].endN > 0) {
					children[h].insertElemAtEnd(newR, e);
				}
				else {
					children[child(i)].insertElemAt(newR, e);
				}
			}
			else {
				int32_t back = (int32_t)ceil((double)(n - n0) / l);

				bool secondDequeExists = false;

				if (children[child(back)].isFull()) {
					if (!children[child(++back)].isEmpty() && k == 2) {
						//children[h].transferElementsToSecond();
						secondDequeExists = true;
					}
				}

				secondDequeExists |= (k == 2 && children[child(back)].endN > 0); // TODO: Highly doubt this works...

				if (k == 2 && child(i) == h && r >= l) {
					children[child(i)].insertElemAtEnd(newR, e);
				}
				else {
					if (i < back) {
						int32_t firstRemoval = children[child(back - 1)].removeLast();
						if (secondDequeExists) {
							children[child(back)].insertElemAtEnd(0, firstRemoval);
						}
						else {
							children[child(back)].insertFirst(firstRemoval);
						}
						for (int32_t j = back - 1; j > i; j--) {
							children[child(j)].insertFirst(children[child(j - 1)].removeLast());
						}
					}
					children[child(i)].insertElemAt(newR, e);
				}

			}

			if (children[h].isEmpty()) {
				if (k == 2 && children[h].endN > 0)
					children[h].transferElementsToStart();
				incH(1);
			}
		}

		n++;
	}

	void insertFirst(int32_t e) {
		return insertElemAt(0, e);
	}

	void insertLast(int32_t e) {
		return insertElemAt(n, e);
	}

	int32_t removeElemAt(int32_t r) {
#if DEBUG
		checkIndexOutOfBounds(r, n, "remove", "TieredVector");
#endif
		if (topLevel && tooEmpty()) {
			halveSize();
		}

		int32_t e;
		bool removeFront = r < n - r;

		if (k == 1) {
			e = elements[child(r)];

			if (removeFront || endN > 0) {
				for (int32_t i = h + r; i > h; i--) {
					elements[i % m] = elements[(i + m - 1) % m];
				}
				incH(1);
			}
			else {
				for (int32_t i = h + r; i < h + n; i++) {
					elements[i % m] = elements[(i + m + 1) % m];
				}
			}
			h = isEmpty() ? 0 : h;
		}
		else {
			int32_t l = (int32_t)pow(m, k - 1);

			int32_t n0 = children[h].size();
			int32_t i = (int32_t)ceil((double)(r + 1 - n0) / l);
			int32_t newR = i == 0 ? r : (r - n0) % l;

			if (removeFront) {
				e = children[child(i)].removeElemAt(newR);

				for (int32_t j = i; j > 0; j--) {
					children[child(j)].insertFirst(children[child(j - 1)].removeLast());
				}

				if (children[h].isEmpty()) {
					if (k == 2 && children[h].endN > 0)
						children[h].transferElementsToStart();
					incH(1);
				}
			}
			else {
				int32_t back = (int32_t)ceil((double)(n - n0) / l);

				if (children[child(back)].endN > 0) {
					if (i == back) {
						e = children[child(i)].removeElemAtEnd(newR);
					}
					else {
						e = children[child(i)].removeElemAt(newR);

						for (int32_t j = i; j < back - 1; j++) {
							children[child(j)].insertLast(children[child(j + 1)].removeFirst());
						}

						children[child(back - 1)].insertLast(children[child(back)].removeElemAtEnd(0));
					}
				}
				else {
					e = children[child(i)].removeElemAt(newR);

					for (int32_t j = i; j < back; j++) {
						children[child(j)].insertLast(children[child(j + 1)].removeFirst());
					}
				}
			}
		}

		n--;
		return e;
	}

	int32_t removeFirst() {
		return removeElemAt(0);
	}

	int32_t removeLast() {
		return removeElemAt(n - 1);
	}

	bool isFull() {
		return n == pow(m, k);
	}

	bool isEmpty() {
		return n == 0;
	}

	string toStringPretty2() {
		if (k > 1)
			throw exception("Can only call toStringPretty2 on tiered vector with k = 1");

		string s = "{ ";

		if (endN > 0) {
			s += to_string(elements[endH]);
		}
		for (int32_t i = 1; i < endN; i++) {
			s += ", " + to_string(elements[childEnd(i)]);
		}

		return s += " }";
	}

	string toStringPretty() {
		string s = "{ ";
		if (k == 1) {
			if (n > 0) {
				s += to_string(elements[h]);
			}
			for (int32_t i = 1; i < n; i++) {
				s += ", " + to_string(elements[child(i)]);
			}
		}
		else {
			if (m > 0) {
				s += children[h].toStringPretty();
			}
			for (int32_t i = 1; i < m; i++) {
				s += ", " + children[child(i)].toStringPretty();
			}
			TieredVector firstChild = children[h];

			if (k == 2 && firstChild.endN > 0) {
				s += ", " + firstChild.toStringPretty2();
			}
		}

		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (k == 1) {
			if (m > 0) {
				s += to_string(elements[0]);
			}
			for (int32_t i = 1; i < m; i++) {
				s += ", " + to_string(elements[i]);
			}
		}
		else {
			if (m > 0) {
				s += children[0].toString();
			}
			for (int32_t i = 1; i < m; i++) {
				s += ", " + children[i].toString();
			}
		}
		//for (int32_t i = 0; i < size(); i++) {
		//	s += to_string(getElemAt(i)) + ", ";
		//}
		return s += " }";
	}
};