#include "stdafx.h"
#include "K2TieredVector.cpp"

class KTieredVector {
private:
	
	int k;
	int h;
	int size;		// Amount of subvectors with elements. Maximum m

	bool topLevel;

	KTieredVector *tv;
	Deque *cd;

	int elem(int r) {
		return (h + r) % l;
	}

	void incH(int inc) {
		h = (h + inc + m) % m;
	}
	
	void init(int k, int size, bool topLevel) {
		this->topLevel = topLevel;
		this->h = 0;
		this->k = k;
		this->m = size;
		this->l = size;
		if (k < 2) {
			throw invalid_argument("k must be at least 2");
		}
		else if (k == 2) {
			cd = new Deque[size];
		}
		else {
			tv = (KTieredVector*)malloc(size * sizeof this);
			for (int i = 0; i < size; i++) {
				KTieredVector* newTv = new KTieredVector(k - 1, size, false);
				tv[i] = *newTv;
			}
		}
	}

	void doubleSize() {
		if (!topLevel)
			return;

		if (k == 2) {
			Deque *newA = new Deque[m * 2];
			for (int i = 0; i < m * 2; i++) {
				delete[](newA[i].a);
				newA[i] = Deque(l * 2);
			}

			// Move everything to the lower 1/4 of the current TV
			int limit = n;
			int moved = 0;
			int cdToRemoveFrom = 0;
			Deque oldCd = cd[elem(cdToRemoveFrom)];
			for (int i = 0; moved < limit; i++) {
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
		}
		else {
			KTieredVector *newB = (KTieredVector*)calloc(m * 2, sizeof this);
			for (int i = 0; i < m * 2; i++) {
				newB[i] = KTieredVector(k - 1, l * 2, false);
			}

			// Move everything to the lower 1/4 of the current TV
			int limit = n;
			int moved = 0;
			int tvToRemoveFrom = 0;
			KTieredVector oldTv = tv[elem(tvToRemoveFrom)];
			for (int i = 0; moved < limit; i++) {
				while (!newB[i].isFull() && moved < limit) {
					newB[i].insertLast(oldTv.removeFirst());
					moved++;
					if (oldTv.isEmpty()) {
						tvToRemoveFrom++;
						oldTv = tv[elem(tvToRemoveFrom)];
					}
				}
			}
			delete[] tv;
			tv = newB;
		}

		m *= 2;
		l *= 2;

		h = 0;
	}

	void halveSize() {
		if (!topLevel)
			return;

		if (k == 2) {
			Deque *newA = new Deque[m / 2];

			for (int i = 0; i < m / 2; i++) {
				delete[](newA[i].a);
				newA[i] = Deque(l / 2);
			}

			int to = 0;
			for (int i = 0; i < n; i++) {
				newA[to].insertElemAt(i % (l / 2), getElemAt(i));
				if (newA[to].isFull())
					to++;
			}

			delete[] cd;
			cd = newA;
		}
		else {
			KTieredVector *newB = (KTieredVector*)calloc(m / 2, sizeof this);
			for (int i = 0; i < m / 2; i++) {
				newB[i] = KTieredVector(2, l / 2, false);
			}

			int to = 0;
			for (int i = 0; i < n; i++) {
				newB[to].insertElemAt(i % (l / 2), getElemAt(i));
				if (newB[to].isFull())
					to++;
			}

			delete[] tv;
			tv = newB;
		}

		m /= 2;
		l /= 2;
		h = 0;
	}

	KTieredVector(int k, int size, bool topLevel) {
		init(k, size, topLevel);
	}

	KTieredVector(int k, int size) {
		init(k, size, true);
	}

	KTieredVector(int k, bool topLevel) {
		init(k, DEFAULT_SIZE, topLevel);
	}

public:
	int n = 0;		// Number of elements
	int m = 0;		// Number of subvectors
	int l = 0;		// Size of subvectors

	KTieredVector(int k) {
		init(k, DEFAULT_SIZE, true);
	}

	~KTieredVector(void) {
		if (k == 2) {
			delete[] cd;
		}
		else {
			for (int i = 0; i < m; i++) {
				delete(&tv[i]);
			}
			delete[] tv;
		}
	}

	int getElemAt(int r) {
		int l0 = k == 2 ? cd[h].n : tv[h].n;
		int i = ceil((double)(r + 1 - l0) / l);
		int newR = i == 0 ? r : (r - l0) % l;

		if (k == 2) {
			return cd[elem(i)].getElemAt(newR);
		} else {
			return tv[elem(i)].getElemAt(newR);
		}
	}
	
	void insertElemAt(int r, int e) {
		cout << "Inserting " << e << " at " << r << ". k: " << k << endl;
		cout << toStringPretty() << endl;
		checkIndexOutOfBounds(r, n + 1, "insert", "TieredVector");
		if (r == n) {
			return insertLast(e);
		}

		if (k == 2) {
			int l0 = cd[h].n;
			int i = ceil((double)(r + 1 - l0) / l);
			int newR = i == 0 ? r : (r - l0) % l;
			
			bool insertFront = r < n - r;

			if (isFull() && ((insertFront && firstFull()) || (!insertFront && lastFull()))) {
				doubleSize();
				int l0 = cd[h].n;
				i = ceil((double)(r + 1 - l0) / l);
				newR = i == 0 ? r : (r - l0) % l;
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
						newR = i == 0 ? cd[h].n : l - 1;
					}
				}
				for (int j = 0; j < i; j++) {
					cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
				}
			}
			else {
				int back = ceil((double)(n - l0) / l);


				if (cd[elem(back)].isFull()) {
					back++;
				}
				for (int j = back; j > i; j--) {
					cd[elem(j)].insertFirst(cd[elem(j - 1)].removeLast());
				}
			}

			cd[elem(i)].insertElemAt(newR, e);
		}
		else {
			int l0 = tv[h].n;
			int i = ceil((double)(r + 1 - l0) / l);
			int newR = i == 0 ? r : (r - l0) % l;

			bool insertFront = r < n - r;

			if (isFull() && ((insertFront && firstFull()) || (!insertFront && lastFull()))) {
				doubleSize();
				int l0 = tv[h].n;
				i = ceil((double)(r + 1 - l0) / l);
				newR = i == 0 ? r : (r - l0) % l;
			}

			if (insertFront) {
				if (tv[h].isFull()) {
					incH(-1);
					i++;
				}
				if (i > 0) {
					newR--;
					if (newR < 0) {
						i--;
						newR = i == 0 ? tv[h].n : l - 1;
					}
				}
				for (int j = 0; j < i; j++) {
					tv[elem(j)].insertLast(tv[elem(j + 1)].removeFirst());
				}
			}
			else {
				int back = ceil((double)(n - l0) / l);

				if (tv[elem(back)].isFull()) {
					back++;
				}
				for (int j = back; j > i; j--) {
					tv[elem(j)].insertFirst(tv[elem(j - 1)].removeLast());
				}
			}

			tv[elem(i)].insertElemAt(newR, e);
		}

		n++;
	}

	void insertFirst(int e) {
		insertElemAt(0, e);
	}

	void insertLast(int e) {
		if (isFull() && lastFull()) {
			doubleSize();
		}

		int i = n / l;
		if (k == 2) {
			if (cd[elem(i)].isFull())
				i++;
			cd[elem(i)].insertLast(e);
		}
		else {
			if (tv[elem(i)].isFull())
				i++;
			tv[elem(i)].insertLast(e);
		}
		n++;
	}

	int removeElemAt(int r) {
		cout << "Removing at " << r << ". k: " << k << endl;
		cout << toStringPretty() << endl;
		if (tooEmpty()) {
			halveSize();
		}

		int e;
		bool removeFront = r < n - r;

		if (k == 2) {
			int l0 = cd[h].n;
			int i = ceil((double)(r + 1 - l0) / l);
			int newR = i == 0 ? r : (r - l0) % l;

			if (removeFront) {
				e = cd[elem(i)].removeElemAt(newR);

				for (int j = i; j > 0; j--) {
					cd[elem(j)].insertFirst(cd[elem(j - 1)].removeLast());
				}

				if (cd[h].isEmpty()) {
					incH(1);
				}

			}
			else {
				e = cd[elem(i)].removeElemAt(newR);			
			
				int back = ceil((double)(n - l0) / l);

				if (cd[elem(back)].isEmpty()) {
					back--;
				}
			
				for (int j = i; j < back; j++) {
					cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
				}
			}
		}
		else {
			int l0 = tv[h].n;
			int i = ceil((double)(r + 1 - l0) / l);
			int newR = i == 0 ? r : (r - l0) % l;

			if (removeFront) {
				e = tv[elem(i)].removeElemAt(newR);

				for (int j = i; j > 0; j--) {
					tv[elem(j)].insertFirst(tv[elem(j - 1)].removeLast());
				}

				if (tv[h].isEmpty()) {
					incH(1);
				}
			}
			else {
				e = tv[elem(i)].removeElemAt(newR);

				int back = ceil((double)(n - l0) / l);

				if (tv[elem(back)].isEmpty()) {
					back--;
				}

				for (int j = i; j < back; j++) {
					tv[elem(j)].insertLast(tv[elem(j + 1)].removeFirst());
				}
			}
		}
		
		n--;
		return e;
	}

	int removeFirst() {
		return removeElemAt(0);
	}

	int removeLast() {
		return removeElemAt(n);
	}

	bool isFull() {
		if (k == 2) {
			return cd[h].n > 0 && cd[(h - 1 + m) % m].n > 0;
		}
		else {
			return tv[h].n > 0 && tv[(h - 1 + m) % m].n > 0;
		}
	}

	bool isEmpty() {
		return n == 0;
	}

	bool lastFull() {
		int i = (h - 1 + m) % m;
		if (k == 2) {
			return cd[i].n == l;
		}
		else {
			return tv[i].n == l;
		}
	}

	bool firstFull() {
		if (k == 2) {
			return cd[h].n == l;
		}
		else {
			return tv[h].n == l;
		}
	}

	bool tooEmpty() {
		return n < m * l / 8;
	}

	string toStringPretty() {
		string s = "{ ";
		if (k == 0) {
			if (m > 0) {
				s += cd[h].toStringPretty();
			}
			for (int i = 1; i < m; i++) {
				s += ", " + cd[elem(i)].toStringPretty();
			}
		}
		else {
			if (m > 0) {
				s += tv[h].toStringPretty();
			}
			for (int i = 1; i < m; i++) {
				s += ", " + tv[elem(i)].toStringPretty();
			}
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (k == 2) {
			if (m > 0) {
				s += cd[0].toString();
			}
			for (int i = 1; i < m; i++) {
				s += ", " + cd[i].toString();
			}
		}
		else {
			if (m > 0) {
				s += tv[0].toString();
			}
			for (int i = 1; i < m; i++) {
				s += ", " + tv[i].toString();
			}
		}
		return s += " }";
	}
};