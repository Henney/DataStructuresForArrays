#include "stdafx.h"
#include "TieredVector.cpp"
#include <vector>

void testInsertion() {
	int size = 1000;
	int seed1 = 1;
	int seed2 = 2;
	
	int* order = new int[size];
	srand(seed2);
	for (int i = 0; i < size; i++) {
		order[i] = rand() % (i + 1);
	}

	vector<int> items = vector<int>();
	srand(seed1);
	for (int i = 0; i < size; i++) {
		int item = rand() % size;
		items.insert(items.begin() + order[i], item);
	}

	K2TieredVector tv3 = K2TieredVector();
	
	srand(seed1);
	for (int i = 0; i < size; i++) {
		int item = rand() % size;
		tv3.insertElemAt(order[i], item);
	}
	for (int i = 0; i < size; i++) {
		if (items[i] != tv3.getElemAt(i)) {
			cout << "Wrong element in K2TieredVector at index " << i << ": " << tv3.getElemAt(i) << endl;
			cout << "Should be: " << items[i] << endl;
			string s;
			cin >> s;
		}
	}

	BitTrickK2TieredVector tv4 = BitTrickK2TieredVector();
	srand(seed1);
	for (int i = 0; i < size; i++) {
		int item = rand() % size;
		tv4.insertElemAt(order[i], item);
	}
	for (int i = 0; i < size; i++) {
		if (items[i] != tv4.getElemAt(i)) {
			cout << "Wrong element in BitTrickK2TieredVector at index " << i << ": " << tv4.getElemAt(i) << endl;
			cout << "Should be: " << items[i] << endl;
			string s;
			cin >> s;
		}
	}

	// Removal
	srand(seed1);
	for (int i = 1; i <= size; i++) {
		order[i] = rand() % i;
	}
	reverse(order, order + size);
	int* removed = new int[size];

	for (int i = 0; i < size; i++) {
		removed[i] = items[order[i]];
		items.erase(items.begin() + order[i]);
	}

	for (int i = 0; i < size; i++) {
		int item = tv3.removeElemAt(order[i]);
		if (removed[i] != item) {
			cout << "Wrong element removed in K2TieredVector at index " << i << ": " << item << endl;
			cout << "Should be: " << removed[i] << endl;
			string s;
			cin >> s;
		}
	}

	for (int i = 0; i < size; i++) {
		int item = tv4.removeElemAt(order[i]);
		if (removed[i] != item) {
			cout << "Wrong element removed in K2TieredVector at index " << i << ": " << item << endl;
			cout << "Should be: " << removed[i] << endl;
			string s;
			cin >> s;
		}
	}

	cout << "done" << endl;
}

//int main()
//{
//	testInsertion();
//
//	string s;
//	cin >> s;
//	return 0;
//}