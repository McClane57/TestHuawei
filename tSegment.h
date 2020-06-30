#pragma once
#include "tPoint.h"
#include "limits.h"
#include <cassert>

class tSegment
{
private:
	tPoint xStart;
	tPoint xFinish;
	bool xRegular = false;
	std::pair<eRegDir, eRegDir> xDirections;
	//regular directions such that the direction 
	//of the segment in between (if the segment is regular both directions are the same) 
	//first is not greater than second
	bool xChecked = false;
	void xCheck();
public:
	tSegment(tPoint const& s, tPoint const& f) : xStart(s), xFinish(f) { assert(!(xStart == xFinish) && "Bad segment"); }
	tSegment(tSegment const& other) :xStart(other.End(0)), xFinish(other.End(1)) {}
	tPoint const& End(int end) const { return end ? xFinish : xStart; }
	bool IsRegular();
	std::pair<eRegDir, eRegDir> Directions();
	int Limit(eRegDir dir) const;
	void SwapEnds() { std::swap(xStart, xFinish); }
};

