#pragma once
#include <list>
#include "tPoint.h"
#include "tSegment.h"

class tTrajectory
{
private:
	std::list<tPoint> xTrajectory;
public:
	tTrajectory() : xTrajectory{} {}
	tTrajectory(tPoint const& start, tPoint const& end) : xTrajectory{ start, end } {}
	tTrajectory(tTrajectory const& other) : xTrajectory{ other.Trajectory() } {}
	std::list<tPoint> const& Trajectory() const { return xTrajectory; }
	void AddPoint(tPoint const& point, bool start = false);
	bool IsRegular();
};

