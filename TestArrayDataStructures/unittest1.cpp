#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\TieredVector\K2TieredVector.cpp"
#include "..\TieredVector\TieredVector.cpp"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define DEBUG false
#define DETAILED_CONFIRM true

#define SIZE 1000
#define SEED1 491
#define SEED2 864
#define SEED3 3938

linear_congruential_engine<uint32_t, 48271, 0, 2147483647> rand_engine(1);
#define RAND_FUNC rand_engine
#define RESEED(x) rand_engine.seed(x)

namespace TestArrayDataStructures
{
	static void testInsertion(ArrayDataStructure& ds, int32_t* order, vector<vector<int32_t>> expected) {
		RESEED(SEED1);
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = RAND_FUNC() % SIZE;
			ds.insertElemAt(order[i], item);
#if DEBUG
			Logger::WriteMessage(("Inserted " + to_string(item) + " at index " + to_string(order[i])).c_str());
			//Logger::WriteMessage(ds.toString().c_str());
			Logger::WriteMessage(ds.toStringPretty().c_str());
			Logger::WriteMessage(("n: " + to_string(ds.size())).c_str());
#endif
			Assert::AreEqual(
				expected[i][order[i]],
				ds.getElemAt(order[i]),
				(L"Wrong element inserted at index " + to_wstring(order[i]) + L": " + to_wstring(ds.getElemAt(order[i])) + L"\n" +
					L"Should be : " + to_wstring(item)).c_str(),
				LINE_INFO()
			);

#if DETAILED_CONFIRM
			for (int32_t j = 0; j < expected[i].size(); j++) {
				string str = "After inserting " + to_string(item) + " at index " + to_string(order[i]) + ", the element at index " +
					to_string(j) + " was incorrect.\n" + ds.toString() + "\n" + ds.toStringPretty();
				wstring str2(str.length(), L' ');
				copy(str.begin(), str.end(), str2.begin());
				Assert::AreEqual(
					expected[i][j],
					ds.getElemAt(j),
					str2.c_str(),
					LINE_INFO()
				);
			}
#endif
		}
		Assert::AreEqual((uint32_t)SIZE, ds.size());
	}

	void testRetrieval(string name, ArrayDataStructure& ds, vector<int32_t> items) {
		Assert::AreEqual(
			SIZE,
			static_cast<int32_t>(items.size()),
			(L"Size is " + to_wstring(items.size()) + L" but should be " + to_wstring(SIZE)).c_str(),
			LINE_INFO()
		);
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = ds[i];
#if DEBUG
			Logger::WriteMessage(("Retrieved element " + to_string(item) + " at index " + to_string(i)).c_str());
			//Logger::WriteMessage(ds.toString().c_str());
			Logger::WriteMessage(ds.toStringPretty().c_str());
#endif
			Assert::AreEqual(
				items[i],
				item,
				(L"Wrong element at " + wstring(name.begin(), name.end()) + L" at index " + to_wstring(i) + L": " + to_wstring(item) + L"\n" +
					L"Should be : " + to_wstring(items[i])).c_str(),
				LINE_INFO()
			);
		}
	}

	void testRemoval(string name, ArrayDataStructure& ds, int32_t* order, vector<int32_t> removed, vector<vector<int32_t>> expected) {
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = ds.removeElemAt(order[i]);
#if DEBUG
			Logger::WriteMessage(("Removed element " + to_string(item) + " from index " + to_string(order[i])).c_str());
			//Logger::WriteMessage(ds.toString().c_str());
			Logger::WriteMessage(ds.toStringPretty().c_str());
#endif
			Assert::AreEqual(
				removed[i],
				item,
				(L"Wrong element removed in " + wstring(name.begin(), name.end()) + L" at index " + to_wstring(order[i]) + L": " + to_wstring(item) + L"\n" +
					L"Should be : " + to_wstring(removed[i]) + L". Removal number " + to_wstring(i)).c_str(),
				LINE_INFO()
			);
#if DETAILED_CONFIRM
			for (int32_t j = 0; j < expected[i].size(); j++) {
				string str = "After removing " + to_string(item) + " at index " + to_string(order[i]) + ", the element at index " +
					to_string(j) + " was incorrect.\n" + ds.toString() + "\n" + ds.toStringPretty();
				wstring str2(str.length(), L' ');
				copy(str.begin(), str.end(), str2.begin());
				Assert::AreEqual(
					expected[i][j],
					ds.getElemAt(j),
					str2.c_str(),
					LINE_INFO());
			}
#endif
		}
		Assert::IsTrue(ds.size() == 0);
	}

	void testArrayDataStructure(ArrayDataStructure &ds, string name, bool remove) {
		int32_t* order = new int32_t[SIZE];
		RESEED(SEED2);
		for (int32_t i = 0; i < SIZE; i++) {
			order[i] = RAND_FUNC() % (i + 1);
		}

		vector<vector<int32_t>> expected;

		vector<int32_t> items;
		RESEED(SEED1);
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = RAND_FUNC() % SIZE;
			items.insert(items.begin() + order[i], item);
			expected.insert(expected.begin() + i, vector<int32_t>(items));
		}

		testInsertion(ds, order, expected);
		testRetrieval(name, ds, items);

		if (!remove)
			return;

		// Setup for removal
		RESEED(SEED3);
		for (int32_t i = 1; i < SIZE; i++) {
			order[i] = RAND_FUNC() % i;
		}
		reverse(order, order + SIZE);

		vector<vector<int32_t>> expectedRemovals;
		vector<int32_t> removed;

		for (int32_t i = 0; i < SIZE; i++) {
			removed.insert(removed.begin() + i, items[order[i]]);
			items.erase(items.begin() + order[i]);
			expectedRemovals.insert(expectedRemovals.begin() + i, vector<int32_t>(items));
		}
		// End setup

		testRemoval(name, ds, order, removed, expectedRemovals);
	}

	void testArrayDataStructure(ArrayDataStructure &ds, string name) {
		testArrayDataStructure(ds, name, true);
	}

	TEST_CLASS(UnitTests) {
	public:
		TEST_METHOD(testDeque) {
			testArrayDataStructure(Deque(true), "Deque");
		}

		TEST_METHOD(testBitTrickDeque) {
			testArrayDataStructure(BitTrickDeque(true), "BitTrickDeque");
		}

		TEST_METHOD(testSimple2TieredVector) {
			testArrayDataStructure(Simple2TieredVector(), "Simple2TieredVector");
		}

		TEST_METHOD(testBitTrickSimple2TieredVector) {
			testArrayDataStructure(BitTrickSimple2TieredVector(), "BitTrickSimple2TieredVector");
		}

		TEST_METHOD(testDeque2TieredVector) {
			testArrayDataStructure(Deque2TieredVector(), "Deque2TieredVector");
		}

		TEST_METHOD(testBitTrickDeque2TieredVector) {
			testArrayDataStructure(BitTrickDeque2TieredVector(), "BitTrickDeque2TieredVector");
		}

		TEST_METHOD(testDeamortisedBitTrickSimple2TieredVector) {
			testArrayDataStructure(DeamortisedBitTrickSimple2TieredVector(), "DeamortisedBitTrickSimple2TieredVector", false);
		}

		//TEST_METHOD(testGeneral2TieredVector) {
		//	testRankSequence(TieredVector(2, 32), "2TieredVector");
		//}

		//TEST_METHOD(testGeneral3TieredVector) {
		//	testRankSequence(TieredVector(3, 4), "3TieredVector");
		//}
	};
}