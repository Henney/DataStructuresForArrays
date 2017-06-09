#pragma once
#include "Deque.h"

#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED
#include "Util.h"
#endif // ! _UTIL_INCLUDED

class K2TieredVector : public RankSequence {
public:
	K2TieredVector(int32_t size);
	K2TieredVector(void);
	~K2TieredVector(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};


class BitTrickK2TieredVector : public RankSequence {
public:
	BitTrickK2TieredVector(void);
	~BitTrickK2TieredVector(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};


class DequeK2TieredVector : public RankSequence {
public:
	DequeK2TieredVector(int32_t size);
	DequeK2TieredVector(void);
	~DequeK2TieredVector(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};