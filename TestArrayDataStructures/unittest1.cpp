#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\TieredVector\K2TieredVector.cpp"
#include "..\TieredVector\TieredVector.cpp"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define DEBUG true

#define SIZE 1000
#define SEED1 1
#define SEED2 2

linear_congruential_engine<uint32_t, 48271, 0, 2147483647> rand_engine(1);
#define RAND_FUNC rand_engine
#define RESEED(x) rand_engine.seed(x)

#define NOOP

namespace TestArrayDataStructures
{
	static void testInsertion(RankSequence& ds, int32_t* order) {
		RESEED(SEED1);
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = RAND_FUNC() % SIZE;
			ds.insertElemAt(order[i], item);
#if DEBUG
			Logger::WriteMessage(("Inserted " + to_string(item) + " at index " + to_string(order[i])).c_str());
			Logger::WriteMessage(ds.toString().c_str());
			Logger::WriteMessage(ds.toStringPretty().c_str());
			Logger::WriteMessage(("n: " + to_string(ds.size())).c_str());
#endif
			Assert::AreEqual(
				ds[order[i]],
				item,
				(L"Wrong element inserted at index " + to_wstring(order[i]) + L": " + to_wstring(ds[order[i]]) + L"\n" +
					L"Should be : " + to_wstring(item) + L". Size is: " + to_wstring(ds.size())).c_str(),
				LINE_INFO());
		}
		Assert::AreEqual(SIZE, ds.size());
	}

	void testRetrieval(string name, RankSequence& ds, vector<int32_t> items) {
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
			Logger::WriteMessage(ds.toString().c_str());
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

	void testRemoval(string name, RankSequence& ds, int32_t* order, int32_t* removed) {
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
		}
		Assert::IsTrue(ds.size() == 0);
	}

	void testRankSequence(RankSequence &ds, string name) {
		int32_t* order = new int32_t[SIZE];
		RESEED(SEED2);
		for (int32_t i = 0; i < SIZE; i++) {
			order[i] = RAND_FUNC() % (i + 1);
		}

		vector<int32_t> items;
		RESEED(SEED1);
		for (int32_t i = 0; i < SIZE; i++) {
			int32_t item = RAND_FUNC() % SIZE;
			items.insert(items.begin() + order[i], item);
		}

		testInsertion(ds, order);
		testRetrieval(name, ds, items);

		// Setup for removal
		RESEED(SEED1);
		for (int32_t i = 1; i < SIZE; i++) {
			order[i] = RAND_FUNC() % i;
		}
		reverse(order, order + SIZE);
		int32_t* removed = new int32_t[SIZE];

		for (int32_t i = 0; i < SIZE; i++) {
			removed[i] = items[order[i]];
			items.erase(items.begin() + order[i]);
		}
		// End setup

		testRemoval(name, ds, order, removed);
	}

	TEST_CLASS(UnitTests) {
	public:
		TEST_METHOD(testDeque) {
			testRankSequence(Deque(true), "Deque");
		}

		TEST_METHOD(testBitTrickDeque) {
			testRankSequence(BitTrickDeque(true), "BitTrickDeque");
		}

		TEST_METHOD(testK2TieredVector) {
			testRankSequence(K2TieredVector(), "K2TieredVector");
		}

		TEST_METHOD(testBitTrickK2TieredVector) {
			testRankSequence(BitTrickK2TieredVector(), "BitTrickK2TieredVector");
		}

		TEST_METHOD(testDequeK2TieredVector) {
			testRankSequence(DequeK2TieredVector(), "DequeK2TieredVector");
		}

		TEST_METHOD(testGeneral2TieredVector) {
			testRankSequence(TieredVector(2, 32), "TieredVector");
		}

		TEST_METHOD(testIndexing) {
			K2TieredVector tv;
			int32_t indexes = 100;
			for (int32_t i = 0; i < 100; i++) {
				tv.insertLast(i);
			}
			Assert::AreEqual(tv.size(), indexes);
			for (int32_t i = 0; i < 100; i++) {
				Assert::AreEqual(tv[i], i);
			}
		}
	};
}