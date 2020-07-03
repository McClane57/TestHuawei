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

bool tTrajectory::operator==(tTrajectory const& other) const
{
	if (xTrajectory.size() != other.Trajectory().size())
		return false;

	for (auto iter = xTrajectory.begin(), jter = other.Trajectory().begin(); iter != xTrajectory.end(); ++iter, ++jter) {
		if (!(*iter == *jter)) {
			return false;
		}
	}
	return true;
}
