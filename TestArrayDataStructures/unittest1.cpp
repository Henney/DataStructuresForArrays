#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\TieredVector\Util.h"
#include "..\TieredVector\K2TieredVector.cpp"
#include "..\TieredVector\TieredVector.cpp"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

linear_congruential_engine<uint32_t, 48271, 0, 2147483647> rand_engine(1);
#define RAND_FUNC rand_engine
#define RESEED(x) rand_engine.seed(x)

#define SIZE 10000
#define SEED1 1
#define SEED2 2

namespace TestArrayDataStructures
{		

	static void testInsertion(RankSequence& ds, int32_t* order) {
		RESEED(SEED1);
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = RAND_FUNC() % SIZE;
			ds.insertElemAt(order[i], item);
		}
	}

	void testRetrieval(string name, RankSequence& ds, vector<int32_t> items) {
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = ds.getElemAt(i);
			Assert::IsTrue(
				items[i] == item,
				(L"Wrong element at " + wstring(name.begin(), name.end()) + L" at index " + to_wstring(i) + L": " + to_wstring(item) + L"\n" +
					L"Should be : " + to_wstring(items[i])).c_str(),
				LINE_INFO()
			);
		}
	}

	void testRemoval(string name, RankSequence& ds, int32_t* order, int32_t* removed) {
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = ds.removeElemAt(order[i]);
			Assert::IsTrue(
				removed[i] == item,
				(L"Wrong element removed in " + wstring(name.begin(), name.end()) + L" at index " + to_wstring(i) + L": " + to_wstring(item) + L"\n" +
				L"Should be : " + to_wstring(removed[i])).c_str(),
				LINE_INFO()
			);
		}
	}

	TEST_CLASS(UnitTest1)
	{
	public:		
		TEST_METHOD(TestInsertionRemoval)
		{
			
			int32_t* order = new int32_t[SIZE];
			RESEED(SEED2);
			for (int32_t i = 0; i < SIZE; i++) {
				order[i] = RAND_FUNC() % (i + 1);
			}
			
			vector<int32_t> items = vector<int32_t>();
			RESEED(SEED1);
			for (int32_t i = 0; i < SIZE; i++) {
				int32_t item = RAND_FUNC() % SIZE;
				items.insert(items.begin() + order[i], item);
			}
			
			Deque deque = Deque(true);
			testInsertion(deque, order);
			testRetrieval("Deque", deque, items);
			
			BitTrickDeque btDeque = BitTrickDeque(true);
			testInsertion(btDeque, order);
			testRetrieval("BitTrickDeque", btDeque, items);
			
			K2TieredVector k2tv = K2TieredVector();
			testInsertion(k2tv, order);
			testRetrieval("K2TieredVector", k2tv, items);
			
			BitTrickK2TieredVector btk2tv = BitTrickK2TieredVector();
			testInsertion(btk2tv, order);
			testRetrieval("BitTrickK2TieredVector", btk2tv, items);
			
			// TODO: Fix
			//DequeK2TieredVector dk2tv = DequeK2TieredVector();
			//testInsertion(dk2tv, order);
			//testRetrieval("DequeK2TieredVector", dk2tv, items);
			
			//TieredVector tv2 = TieredVector(2);
			//testInsertion(tv2, order);
			//testRetrieval("TieredVector(2)", tv2, items);
			
			// Setup for removal
			RESEED(SEED1);
			for (int32_t i = 1; i < SIZE; i++) { // TODO: goes out of the bounds of order
				order[i] = RAND_FUNC() % i;
			}
			reverse(order, order + SIZE);
			int32_t* removed = new int32_t[SIZE];
			
			for (int32_t i = 0; i < SIZE; i++) {
				removed[i] = items[order[i]];
				items.erase(items.begin() + order[i]);
			}
			// End setup
			
			// Removal
			
			testRemoval("Deque", deque, order, removed);
			testRemoval("BitTrickDeque", btDeque, order, removed);
			testRemoval("K2TieredVector", k2tv, order, removed);
			testRemoval("BitTrickK2TieredVector", btk2tv, order, removed);
			//testRemoval("DequeK2TieredVector", dk2tv, order, removed);
		}
	};
}