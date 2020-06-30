#pragma once
#include <map>
#include "tPoint.h"
#include "tSegment.h"

class tShadow
{
private:
	std::map<int, int> xShadow;
	tSegment xDiagonal;
	void xInit();
public:
	tShadow(tPoint start, tPoint end) : xDiagonal(start, end) { xInit(); }
};

