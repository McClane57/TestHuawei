#pragma once
#include "tPoint.h"
#include "tSegment.h"
#include <array>

class tOctet
{
private:
	std::array<int,8> xLimits;
	void xCure(); //
	void xNeighbourCommonPoint(eRegDir dir1, eRegDir dir2, tPoint& point);
public:
	// Constructors
	tOctet() : xLimits{0,0,0,0,0,0,0,0} {}
	tOctet(const tOctet& octet) : 
		xLimits{octet.Limit(E), octet.Limit(NE), octet.Limit(N), octet.Limit(NW), octet.Limit(W), octet.Limit(SW), octet.Limit(S), octet.Limit(SE)} 
	{}

	tOctet(const std::array<int, 8>& lim) : xLimits(lim) {}
	tOctet(const tPoint& point) : 
		xLimits{point.Limit(E), point.Limit(NE), point.Limit(N), point.Limit(NW), point.Limit(W), point.Limit(SW), point.Limit(S), point.Limit(SE)}
	{}


	tOctet(const tSegment & segment) :
		xLimits{ segment.Limit(E), segment.Limit(NE), segment.Limit(N), segment.Limit(NW), segment.Limit(W), segment.Limit(SW), segment.Limit(S), segment.Limit(SE) }
	{}

	int Limit(eRegDir dir) const { return xLimits[dir]; }

	void CoverPoint(const tPoint& point);

	int IsInside(const tPoint& point) const;

	bool IsFull() const; // if all the edges are non-empty

	bool HasEdge(eRegDir dir) const;

	bool CommonPoint(eRegDir dir1, eRegDir dir2, tPoint & result); // returns false if the directions are not neighbour (no point computed)

	tPoint Vertex(eRegDir dir);

	bool IsInflateble(int difference) const;

	tOctet& Inflate(int difference);

	bool IsPoint();

	bool IsRegularSegment();
};


std::ostream& operator<<(std::ostream& os, const tOctet& octet);