#pragma once

#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED
#include "Util.h"
#endif // ! _UTIL_INCLUDED

class Deque : public RankSequence {
public:
	Deque(int32_t, bool);
	Deque(int32_t);
	Deque(bool);
	Deque(void);
	
	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};


class BitTrickDeque : public RankSequence {
public:
	BitTrickDeque(int32_t, bool);
	BitTrickDeque(int32_t);
	BitTrickDeque(bool);
	BitTrickDeque(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};