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
	int xWidth; //half of width
	//regular directions such that the direction 
	//of the segment in between (if the segment is regular both directions are the same) 
	//first is not greater than second
	bool xChecked = false;
	void xCheck();
public:
	tSegment(tPoint const& start, eRegDir dir, int limit, int width = 0);
	tSegment(tPoint const& s, tPoint const& f, int width = 0) : xStart(s), xFinish(f), xWidth(width) { assert(!(xStart == xFinish) && "Bad segment"); }
	tSegment(tSegment const& other, int width = 0) :xStart(other.End(0)), xFinish(other.End(1)), xWidth(width) {}
	tPoint const& End(int end) const { return end ? xFinish : xStart; }
	bool IsRegular();
	std::pair<eRegDir, eRegDir> Directions();
	int Limit(eRegDir dir) const;
	void SwapEnds();
	int Width() const { return xWidth; }
	//for the next functions we assume that the segment is correctly ordered and nonintersected;
	int IsAlign(tSegment const& other) const; //returns 0 if align, 1 if above and -1 if under
	int IsPointOnLine(tPoint const& point) const; //returns 0 if align, 1 if above and -1 if under

};

bool Less(tSegment const& first, tSegment const& second);