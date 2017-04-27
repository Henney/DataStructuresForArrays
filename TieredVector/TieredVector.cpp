#include "stdafx.h"
#include "K2TieredVector.cpp"

class KTieredVector {
private:
	
	int k;
	int h;
	int size;		// Amount of subvectors with elements. Maximum m

	KTieredVector *tv;
	Deque *cd;

	int elem(int r) {
		return (h + r) % l;
	}

	void incH(int inc) {
		h = (h + inc + m) % m;
	}
	
	void init(int k) {
		this->h = 0;
		this->k = k;
		this->m = DEFAULT_SIZE;
		this->l = DEFAULT_SIZE;
		if (k < 2) {
			throw invalid_argument("k must be at least 2");
		}
		else if (k == 2) {
			cd = new Deque[DEFAULT_SIZE];
		}
		else {
			tv = new KTieredVector[DEFAULT_SIZE] { k - 1, k - 1, k - 1, k - 1 };
		}
	}

	void doubleSize() {
		cout << "Doubling size" << endl;

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
			// TODO
		}

		m *= 2;
		l *= 2;

		h = 0;
	}

	void halveSize() {
		Deque *newA = new Deque[m / 2];

		for (int i = 0; i < m / 2; i++) {
			delete[](newA[i].a);
			newA[i] = Deque(l / 2);
		}

		for (int i = 0; i < m / 4; i++) {
			newA[i] = cd[elem(i / 2)].lowerHalf();
			i++;
			newA[i] = cd[elem(i / 2)].upperHalf(); // TODO: elem(i / 2) doesn't quite seem right. Might be a bug
		}
		delete[] cd;
		cd = newA;

		m /= 2;
		l /= 2;
	}

public:
	int n = 0;		// Number of elements
	int m = 0;		// Number of subvectors
	int l = 0;		// Size of subvectors

	KTieredVector(int k) {
		init(k);
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
			cout << "l0: " << l0 << " i: " << i << " newR: " << newR << endl;

			if (insertFront) {
				cout << "Inserting from front" << endl;
				if (cd[h].isFull()) {
					cout << "front is full - decrementing head" << endl;
					incH(-1);
					i++;
				}
				if (i > 0) {
					newR--;
					if (newR < 0) {
						i--;
						newR = i == 0 ? cd[h].n : l - 1;
						cout << "newR is 0 so inserting at the end of the previous vector instead. i: " << i << " newR: " << newR << endl;
					}
				}
				for (int j = 0; j < i; j++) {
					cout << "Moving element backward" << endl;
					cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
					cout << "Done moving" << endl;
				}
			}
			else {
				cout << "Inserting from back" << endl;
				int back = ceil((double)(n - l0) / l); // TODO: This is wrong


				if (cd[elem(back)].isFull()) {
					cout << "back is full - incrementing back" << endl;
					back++;
				}
				for (int j = back; j > i; j--) {
					cout << "Moving element forward" << endl;
					cd[elem(j)].insertFirst(cd[elem(j - 1)].removeLast());
					cout << "Done moving" << endl;
				}
			}

			cout << toStringPretty() << endl;
			cout << "i: " << i << " newR: " << newR << endl;
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
			cout << "l0: " << l0 << " i: " << i << " newR: " << newR << endl;

			if (insertFront) {
				cout << "Inserting from front" << endl;
				if (tv[h].isFull()) {
					cout << "front is full - decrementing head" << endl;
					incH(-1);
					i++;
				}
				if (i > 0) {
					newR--;
					if (newR < 0) {
						i--;
						newR = i == 0 ? tv[h].n : l - 1;
						cout << "newR is 0 so inserting at the end of the previous vector instead. i: " << i << " newR: " << newR << endl;
					}
				}
				for (int j = 0; j < i; j++) {
					cout << "Moving element backward" << endl;
					tv[elem(j)].insertLast(tv[elem(j + 1)].removeFirst());
					cout << "Done moving" << endl;
				}
			}
			else {
				cout << "Inserting from back" << endl;
				int back = ceil((double)(n - l0) / l); // TODO: This is wrong


				if (tv[elem(back)].isFull()) {
					cout << "back is full - incrementing back" << endl;
					back++;
				}
				for (int j = back; j > i; j--) {
					cout << "Moving element forward" << endl;
					tv[elem(j)].insertFirst(tv[elem(j - 1)].removeLast());
					cout << "Done moving" << endl;
				}
			}

			cout << toStringPretty() << endl;
			cout << "i: " << i << " newR: " << newR << endl;
			tv[elem(i)].insertElemAt(newR, e);
		}

		n++;
	}

	void insertFirst(int e) {
		insertElemAt(0, e);
	}

	void insertLast(int e) {
		cout << "isFull: " << isFull() << " lastFull: " << lastFull() << endl;
		if (isFull() && lastFull()) {
			doubleSize();
		}

		int i = n / l;
		if (cd[elem(i)].isFull())
			i++;
		cd[elem(i)].insertLast(e);
		n++;
	}

	int removeElemAt(int r) {
		if (tooEmpty()) {
			halveSize();
		}

		if (k == 2) {
			int l0 = cd[h].n;
			int i = ceil((double)(r + 1 - l0) / l);
			int newR = i == 0 ? r : (r - l0) % l;

			bool removeFront = r < n - r;

			if (removeFront) {
				cout << "Removing from front" << endl;
				if (cd[h].isFull()) {
					cout << "front is full - decrementing head" << endl;
					incH(-1);
					i++;
				}
				if (i > 0) {
					newR--;
					if (newR < 0) {
						i--;
						newR = i == 0 ? cd[h].n : l - 1;
						cout << "newR is 0 so inserting at the end of the previous vector instead. i: " << i << " newR: " << newR << endl;
					}
				}
				for (int j = 0; j < i; j++) {
					cout << "Moving element backward" << endl;
					cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
					cout << "Done moving" << endl;
				}
			}
			else {
				cout << "Inserting from back" << endl;
				int back = ceil((double)(n - l0) / l); // TODO: This is wrong


				if (cd[elem(back)].isFull()) {
					cout << "back is full - incrementing back" << endl;
					back++;
				}
				for (int j = back; j > i; j--) {
					cout << "Moving element forward" << endl;
					cd[elem(j)].insertFirst(cd[elem(j - 1)].removeLast());
					cout << "Done moving" << endl;
				}
			}

			cout << toStringPretty() << endl;
			cout << "i: " << i << " newR: " << newR << endl;
			//cd[elem(i)].insertElemAt(newR, e);
		}

		int i = (r - 1) / m;
		int e = cd[elem(i)].removeElemAt(r - i*m);
		for (int j = i; j < (n - 1) / m; j++) {
			cd[elem(j)].insertLast(cd[elem(j + 1)].removeFirst());
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

	bool isFull() { // Needs a good definition
		if (k == 2) {
			return cd[h].n > 0 && cd[(h - 1 + m) % m].n > 0;
		}
		else {
			return tv[h].n > 0 && tv[(h - 1 + m) % m].n > 0;
		}
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
		if (m > 0) {
			s += cd[h].toStringPretty();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + cd[elem(i)].toStringPretty();
		}
		return s += " }";
	}

	string toString() {
		string s = "{ ";
		if (m > 0) {
			s += cd[0].toString();
		}
		for (int i = 1; i < m; i++) {
			s += ", " + cd[i].toString();
		}
		return s += " }";
	}
};