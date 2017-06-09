#include "stdafx.h"

#include "Util.h"

// TODO: Make a version that utilises bitshifts
class TieredVector : public RankSequence {
private:
	int32_t k;
	int32_t m = 0;		// Number of children
	bool topLevel = false;

	int32_t n = 0;
	int32_t h = 0;

	int32_t h2 = 0;
	int32_t n2 = 0;

	vector<TieredVector> children;
	int32_t* elements;

	int32_t elem(int32_t r) {
		return (h + r) % m;
	}

	int32_t elem2(int32_t r) {
		if (k > 1)
			throw exception("Can only call elem2 on a 1-tiered vector");
		return (h2 + r) % m;
	}

	void incH(int8_t inc) {
		h = (h + inc + m) % m;
	}

	void incH2(int8_t inc) {
		if (k > 1)
			throw exception("Can only call incH2 on a 1-tiered vector");
		h2 = (h2 + inc + m) % m;
	}

	void init(int32_t k, int32_t size, bool topLevel) {
		this->k = k;
		this->m = size;
		this->topLevel = topLevel;

		if (k < 1) {
			throw invalid_argument("k must be a positive integer");
		}
		else if (k == 1) {
			elements = (int32_t*)calloc(m, sizeof 0);
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

	int32_t getElemAt2(int32_t r) {
		if (k > 1)
			throw exception("Can only call getElemAt2 on 1-tiered vector");
		return elements[elem2(r)];
	}

	void insertElemAt2(int32_t r, int32_t e) {
		if (k > 1)
			throw exception("Can only call insertElemAt2 on 1-tiered vector");
		if (n + n2 == m)
			throw exception("Cannot insert if the deque is already full");

		if (n2 == 0) {
			h2 = (h + n) % m;
		}
		for (int32_t i = h2 + n2; i > h2 + r; i--) {
			elements[(i + m) % m] = elements[(i + m - 1) % m];
		}
		elements[elem2(r)] = e;
	}

	int32_t removeElemAt2(int32_t r) {
		if (k > 1)
			throw exception("Can only call removeElemAt2 on 1-tiered vector");

		if (topLevel && tooEmpty()) {
			halveSize();
		}

		int32_t e = elements[elem2(r)];

		if (r < n - r) {
			for (int32_t i = h + r; i > h; i--) {
				elements[i % m] = elements[(i + m - 1) % m];
			}
			incH2(1);
		}
		else {
			for (int32_t i = h + r; i < h + n; i++) {
				elements[i % m] = elements[(i + m + 1) % m];
			}
		}
		n--;
		h2 = n2 == 0 ? 0 : h2;
		return e;
	}

	void transferElementsToSecond() {
		if (k > 1)
			throw exception("Can only call transferElementsToSecond on 1-tiered vector");
		if (n2 != 0)
			throw exception("There must not be elements in the second part of the deque when transferring");
		h2 = h;
		n2 = n;
		incH(-1);
		n = 0;
	}

	void transferElementsToFirst() {
		if (k > 1)
			throw exception("Can only call transferElementsToFirst on 1-tiered vector");
		if (n != 0)
			throw exception("There must not be elements in the first part of the deque when transferring");
		h = h2;
		n = n2;
		n2 = 0;
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

	//TieredVector(TieredVector const& copy)
	//{
	//	k = copy.k;
	//	m = copy.m;

	//	if (k == 1) {
	//		elements = (int32_t*)calloc(m, sizeof 0);
	//		std::copy(&copy.elements[0], &copy.elements[copy.m], elements);
	//	}
	//	else {
	//		children = (TieredVector*)calloc(m, sizeof this);
	//		std::copy(&copy.children[0], &copy.children[copy.m], children);
	//	}
	//}

	////TieredVector& operator=(TieredVector rhs) // Pass by value (thus generating a copy)
	////{
	////	rhs.swap(*this); // Now swap data with the copy.
	////					 // The rhs parameter will delete the array when it
	////					 // goes out of scope at the end of the function
	////	return *this;
	////}

	//void swap(TieredVector& s) noexcept
	//{
	//	using std::swap;
	//	swap(this->m, s.m);
	//	swap(this->elements, s.elements);
	//	swap(this->children, s.children);
	//}

	//// C++11
	//TieredVector(TieredVector&& src) noexcept
	//	: m(0)
	//	, elements(NULL)
	//	, children(NULL)
	//{
	//	src.swap(*this);
	//}
	//TieredVector& operator=(TieredVector&& src) noexcept
	//{
	//	src.swap(*this);     // You are moving the state of the src object
	//						 // into this one. The state of the src object
	//						 // after the move must be valid but indeterminate.
	//						 //
	//						 // The easiest way to do this is to swap the states
	//						 // of the two objects.
	//						 //
	//						 // Note: Doing any operation on src after a move 
	//						 // is risky (apart from destroy) until you put it 
	//						 // into a specific state. Your object should have
	//						 // appropriate methods for this.
	//						 // 
	//						 // Example: Assignment (operator = should work).
	//						 //          std::vector() has clear() which sets
	//						 //          a specific state without needing to
	//						 //          know the current state.
	//	return *this;
	//}

	~TieredVector(void) {
		if (k == 1) {
			//delete[] elements;
		}
		else {
			//delete &children;
		}
	}

	int32_t size() {
		return n;
	}

	int32_t getElemAt(int32_t r) {
		if (k == 1) {
			return elements[elem(r)];
		}
		else {
			int32_t l0 = children[h].size();
			int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
			int32_t newR = i == 0 ? r : (r - l0) % m;

			int32_t idx = elem(i);

			if (k == 2 && idx == h && r >= m) {
				return children[idx].getElemAt2(newR);
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
		//	int32_t l0 = children[h].size();
		//	int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
		//	int32_t newR = i == 0 ? r : (r - l0) % m;

		//	children[elem(i)].setElemAt(newR, e);
		//}
	}

	void insertElemAt(int32_t r, int32_t e) {
#if DEBUG
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
#endif
		//if (r == n) {
		//	return insertLast(e);
		//}
		//else if (r == 0) {
		//	return insertFirst(e);
		//}

		if (isFull()) {
			doubleSize();
		}

		bool insertFront = r < n - r;

		if (k == 1) {
			if (insertFront || n2 > 0) { // Always have to insert from the front if the deque has "two" deques
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
			// TODO: Do fancy stuff here
			int32_t n0 = children[h].size();
			int32_t i = (int32_t)ceil((double)(r + 1 - n0) / m);
			int32_t newR = i == 0 ? r : (r - n0) % m;

			if (insertFront) {
				if (children[h].isFull()) {
					incH(-1);
					i++;
					if (!children[h].isEmpty()) {
						children[h].transferElementsToSecond();
					}
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

				children[elem(i)].insertElemAt(newR, e);
			}
			else {
				int32_t back = (int32_t)ceil((double)(n - n0) / m);

				bool secondDequeExists = false;

				if (children[elem(back)].isFull()) {
					if (k == 2 && !children[elem(++back)].isEmpty()) {
						secondDequeExists = true;
					}
				}

				secondDequeExists |= k == 2 && children[elem(back)].n2 > 0; // TODO: Highly doubt this works...
				
				if (k == 2 && elem(i) == h && r >= m) {
					children[elem(back)].insertElemAt2(newR, e);
				}
				else {
					if (i < back) {
						int32_t firstRemoval = children[elem(back - 1)].removeLast();
						if (secondDequeExists) {
							children[elem(back)].insertElemAt2(0, firstRemoval);
						}
						else {
							children[elem(back)].insertFirst(firstRemoval);
						}
						for (int32_t j = back - 1; j > i; j--) {
							children[elem(j)].insertFirst(children[elem(j - 1)].removeLast());
						}
					}
					children[elem(i)].insertElemAt(newR, e);
				}

			}


			if (children[h].isEmpty()) {
				incH(1);
			}
		}

		n++;
	}

	void insertFirst(int32_t e) {
		return insertElemAt(0, e);
		// TODO: Make sure the below works
		if (isFull()) {
			doubleSize();
		}

		if (k == 1) {
			incH(-1);
			elements[h] = e;
		}
		else {
			if (children[h].isFull())
				h--;
			children[h].insertFirst(e);
		}
		n++;
	}

	void insertLast(int32_t e) {
		return insertElemAt(n, e);
		// TODO: Make sure the below works
		if (isFull()) {
			doubleSize();
		}

		if (k == 1) {
			elements[(h + n) % m] = e;
		}
		else {
			int32_t i = n / m;
			if (children[elem(i)].isFull())
				i++;
			children[elem(i)].insertLast(e);
		}
		n++;
	}

	int32_t removeElemAt(int32_t r) {
		if (topLevel && tooEmpty()) {
			halveSize();
		}

		int32_t e;
		bool removeFront = r < n - r;

		if (k == 1) {
			e = elements[elem(r)];

			if (removeFront || n2 > 0) {
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
			int32_t l0 = children[h].size();
			int32_t i = (int32_t)ceil((double)(r + 1 - l0) / m);
			int32_t newR = i == 0 ? r : (r - l0) % m;

			if (removeFront) {
				e = children[elem(i)].removeElemAt(newR);

				for (int32_t j = i; j > 0; j--) {
					children[elem(j)].insertFirst(children[elem(j - 1)].removeLast());
				}

				if (children[h].isEmpty()) {
					if (k == 2 && children[h].n2 > 0)
						children[h].transferElementsToFirst();
					incH(1);
				}
			}
			else {
				int32_t back = (int32_t)ceil((double)(n - l0) / m);

				if (k == 2 && back == i && children[elem(i)].n2 > 0) {
					e = children[elem(i)].removeElemAt2(newR);
				}
				else {
					e = children[elem(i)].removeElemAt(newR);
				}				

				if (children[elem(back)].isEmpty()) {
					back--;
				}

				for (int32_t j = i; j < back; j++) {
					children[elem(j)].insertLast(children[elem(j + 1)].removeFirst());
				}
			}
		}

		n--;
		//if (k == 2 && n == 0) {
		//	return h;
		//}
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

		if (n2 > 0) {
			s += to_string(elements[h2]);
		}
		for (int32_t i = 1; i <= n2; i++) {
			s += ", " + to_string(elements[elem2(i)]);
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
				s += ", " + to_string(elements[elem(i)]);
			}
		}
		else {
			if (m > 0) {
				s += children[h].toStringPretty();
			}
			for (int32_t i = 1; i < m; i++) {
				s += ", " + children[elem(i)].toStringPretty();
			}
			TieredVector firstChild = children[h];
			if (k == 2 && firstChild.n2 > 0) {
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
		return s += " }";
	}
};