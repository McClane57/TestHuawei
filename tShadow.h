#pragma once
#include <map>
#include "tPoint.h"
#include "tSegment.h"
#include "tOctet.h"
#include "tTrajectory.h"

class tShadow
{
private:
	std::map<int, int> xMap;
	eRegDir xValueDir;
	eRegDir xKeyDir;
	std::map<int, int>::const_iterator xIfAddable(int key, int val) const;
	void xRemoveExtra(int inserted_key);
	int xReturnLimit(tPoint const& point) const;
public:
	tShadow(): xValueDir(E), xKeyDir(E) {}
	tShadow(eRegDir key, eRegDir value, tPoint const & end);
	tShadow operator = (tShadow const& shadow);

	bool AddObstacle(tOctet const& octet);
	tPoint NextPoint(tPoint const& start) const;
	std::map<int, int> const& Map() const { return xMap; }
};

class tLadder
{
private:
	tShadow xShadow;
	tSegment xDiagonal;
	std::pair<eRegDir, eRegDir> xDirections;
	void xInit();
public:
	tLadder(tSegment const& segment) : xDiagonal(segment) { xInit(); }
	tLadder(tPoint start, tPoint end) : xDiagonal(start, end) { xInit(); }
	void AddObstacle(tOctet const& octet) { xShadow.AddObstacle(octet); }
	tPoint LastPointUnderDiagonal(tPoint const& start) const;
	tTrajectory MakeLadder() const;
};

