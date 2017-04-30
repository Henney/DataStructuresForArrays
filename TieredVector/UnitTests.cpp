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

	//K2TieredVector k2tv = K2TieredVector();
	//
	//srand(seed1);
	//for (int i = 0; i < size; i++) {
	//	int item = rand() % size;
	//	k2tv.insertElemAt(order[i], item);
	//}
	//for (int i = 0; i < size; i++) {
	//	if (items[i] != k2tv.getElemAt(i)) {
	//		cout << "Wrong element in K2TieredVector at index " << i << ": " << k2tv.getElemAt(i) << endl;
	//		cout << "Should be: " << items[i] << endl;
	//		string s;
	//		cin >> s;
	//	}
	//}

	//BitTrickK2TieredVector btk2tv = BitTrickK2TieredVector();
	//srand(seed1);
	//for (int i = 0; i < size; i++) {
	//	int item = rand() % size;
	//	btk2tv.insertElemAt(order[i], item);
	//}
	//for (int i = 0; i < size; i++) {
	//	if (items[i] != btk2tv.getElemAt(i)) {
	//		cout << "Wrong element in BitTrickK2TieredVector at index " << i << ": " << btk2tv.getElemAt(i) << endl;
	//		cout << "Should be: " << items[i] << endl;
	//		string s;
	//		cin >> s;
	//	}
	//}

	//KTieredVector tv2 = KTieredVector(2);
	//srand(seed1);
	//for (int i = 0; i < size; i++) {
	//	int item = rand() % size;
	//	tv2.insertElemAt(order[i], item);
	//}
	//for (int i = 0; i < size; i++) {
	//	if (items[i] != tv2.getElemAt(i)) {
	//		cout << "Wrong element in KTieredVector of size 2 at index " << i << ": " << tv2.getElemAt(i) << endl;
	//		cout << "Should be: " << items[i] << endl;
	//		string s;
	//		cin >> s;
	//	}
	//}

	KTieredVector tv3 = KTieredVector(3);
	srand(seed1);
	for (int i = 0; i < size; i++) {
		int item = rand() % size;
		cout << "Inserting " << item << " at " << order[i] << endl;
		tv3.insertElemAt(order[i], item);
	}
	for (int i = 0; i < size; i++) {
		if (items[i] != tv3.getElemAt(i)) {
			cout << "Wrong element in KTieredVector of size 3 at index " << i << ": " << tv3.getElemAt(i) << endl;
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

	//for (int i = 0; i < size; i++) {
	//	int item = k2tv.removeElemAt(order[i]);
	//	if (removed[i] != item) {
	//		cout << "Wrong element removed in K2TieredVector at index " << i << ": " << item << endl;
	//		cout << "Should be: " << removed[i] << endl;
	//		string s;
	//		cin >> s;
	//	}
	//}

	//for (int i = 0; i < size; i++) {
	//	int item = btk2tv.removeElemAt(order[i]);
	//	if (removed[i] != item) {
	//		cout << "Wrong element removed in BitTrickK2TieredVector at index " << i << ": " << item << endl;
	//		cout << "Should be: " << removed[i] << endl;
	//		string s;
	//		cin >> s;
	//	}
	//}

	//for (int i = 0; i < size; i++) {
	//	int item = tv2.removeElemAt(order[i]);
	//	if (removed[i] != item) {
	//		cout << "Wrong element removed in KTieredVector of size 2 at index " << i << ": " << item << endl;
	//		cout << "Should be: " << removed[i] << endl;
	//		string s;
	//		cin >> s;
	//	}
	//}

	cout << "done" << endl;
}

int main()
{
	testInsertion();

	string s;
	cin >> s;
	return 0;
}