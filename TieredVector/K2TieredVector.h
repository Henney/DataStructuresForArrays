#pragma once
#include "Deque.h"

#ifndef _UTIL_INCLUDED
#define _UTIL_INCLUDED
#include "Util.h"
#endif // ! _UTIL_INCLUDED

class Simple2TieredVector : public ArrayDataStructure {
public:
	Simple2TieredVector(int32_t size);
	Simple2TieredVector(void);
	~Simple2TieredVector(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};


class BitTrickSimple2TieredVector : public ArrayDataStructure {
public:
	BitTrickSimple2TieredVector(void);
	~BitTrickSimple2TieredVector(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};


class Deque2TieredVector : public ArrayDataStructure {
public:
	Deque2TieredVector(int32_t size);
	Deque2TieredVector(void);
	~Deque2TieredVector(void);

	int32_t getElemAt(int32_t);
	void insertElemAt(int32_t, int32_t);
	int32_t removeElemAt(int32_t);

	void insertLast(int32_t);
	int32_t removeLast(void);
};