#include "tTrajectory.h"

void tTrajectory::AddPoint(tPoint const& point, bool start)
{
	start ? xTrajectory.push_front(point) : xTrajectory.push_back(point);
	return;
}

bool tTrajectory::IsRegular()
{
	auto iter = xTrajectory.begin();
	for (auto jter = iter++; iter != xTrajectory.end(); jter = iter++)
	{
		tSegment segment(*jter, *iter);
		if (!segment.IsRegular()) {
			return false;
		}
	}
	return true;
}
