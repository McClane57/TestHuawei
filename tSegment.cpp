#include "tSegment.h"
#include <cassert>
#include <boost/multiprecision/cpp_int.hpp>
#include <algorithm>

typedef boost::multiprecision::int128_t tInt128;

std::pair<eRegDir, eRegDir> DirectionsOfSegment(long long diff_x, long long diff_y)
{
	auto result = std::make_pair<eRegDir, eRegDir>(N, N);
	if (diff_x == 0){
		if (diff_y > 0){
			return std::make_pair<eRegDir, eRegDir>(N, N);
		}
		else if (diff_y < 0) {
			return std::make_pair<eRegDir, eRegDir>(S, S);
		}
	}
	else if (diff_y == 0) {
		if (diff_x > 0) {
			return std::make_pair<eRegDir, eRegDir>(E, E);
		}
		else if (diff_x < 0) {
			return std::make_pair<eRegDir, eRegDir>(W, W);
		}
	}
	else if (diff_x > 0) {
		if (diff_y > 0){
			if (diff_x == diff_y){
				result =  std::make_pair<eRegDir, eRegDir>(NE, NE);
			}
			else if (diff_x > diff_y){
				result =  std::make_pair<eRegDir, eRegDir>(E, NE);
			}
			else{
				result =  std::make_pair<eRegDir, eRegDir>(NE,N);
			}
		}
		else if (diff_y < 0) {
			auto directions = DirectionsOfSegment(diff_x, -diff_y);
			result =  std::make_pair<eRegDir, eRegDir>(NegativeDirection(directions.second), NegativeDirection(directions.first));
		}
	}
	else if (diff_x < 0) {
		if (diff_y > 0) {
			auto directions = DirectionsOfSegment(-diff_x, -diff_y);
			result =  std::make_pair<eRegDir, eRegDir>(OppositeDirection(directions.first),
				OppositeDirection(directions.second));
		}
		else if (diff_y < 0) {
			auto directions = DirectionsOfSegment(-diff_x, -diff_y);
			result =  std::make_pair<eRegDir, eRegDir>(OppositeDirection(directions.first),
				OppositeDirection(directions.second));
		}
	}
	assert((result.second == Next(result.first) ||
		result.second == result.first) && "bad directions");
	return result;
}

void tSegment::xCheck()
{
	long long diff_x = static_cast<long long>(xFinish.x) - xStart.x;
	long long diff_y = static_cast<long long>(xFinish.y) - xStart.y;
	xDirections = DirectionsOfSegment(diff_x, diff_y);
	xRegular = (xDirections.first == xDirections.second);
}

tSegment::tSegment(tPoint const& start, eRegDir dir, int limit, int width):xWidth(width), xStart(start)
{
	switch (dir) {
	case E:
		xFinish = tPoint(start.x + limit, start.y);
		return;
	case NE:
		assert(limit % 2 == 0 && "bad diagonal limit");
		xFinish = tPoint(start.x + static_cast<int>(limit/2), start.y + static_cast<int>(limit / 2));
		return;
	case N:
		xFinish = tPoint(start.x, start.y+ limit);
		return;
	case NW:
		assert(limit % 2 == 0 && "bad diagonal limit");
		xFinish = tPoint(start.x - static_cast<int>(limit / 2), start.y + static_cast<int>(limit / 2));
		return;
	case W:
		xFinish = tPoint(start.x - limit, start.y);
		return;
	case SW:
		assert(limit % 2 == 0 && "bad diagonal limit");
		xFinish = tPoint(start.x - static_cast<int>(limit / 2), start.y - static_cast<int>(limit / 2));
		return;
	case S:
		xFinish = tPoint(start.x, start.y - limit);
		return;
	case SE:
		assert(limit % 2 == 0 && "bad diagonal limit");
		xFinish = tPoint(start.x + static_cast<int>(limit / 2), start.y - static_cast<int>(limit / 2));
		return;
	default:
		assert(true && "bad dir");
	}
}

bool tSegment::operator==(tSegment const& other)
{
	if (End(0) == other.End(0) && End(1) == other.End(1))
		return true;
	return (End(1) == other.End(0) && End(0) == other.End(1));
}

bool tSegment::IsRegular()
{
	if (xChecked) {
		return xRegular;
	}
	else {
		xCheck();
		return xRegular;
	}
}

std::pair<eRegDir, eRegDir> tSegment::Directions()
{
	if (xChecked) {
		return xDirections;
	}
	else {
		xCheck();
		return xDirections;
	}
}

int tSegment::Limit(eRegDir dir) const
{
	auto lim_s = xStart.Limit(dir);
	auto lim_f = xFinish.Limit(dir);

	return lim_s > lim_f ? lim_s : lim_f;
}

void tSegment::SwapEnds()
{
	std::swap(xStart, xFinish);
	xChecked = false;
}

int tSegment::IsAlign(tSegment const& other) const
{
	auto p1 = IsPointOnLine(other.End(0));
	auto p2 = IsPointOnLine(other.End(1));
	if (p1 == 0)
		return p2;
	if (p2 == 0)
		return p1;
	if (p1 == p2)
		return p2;
	else
		return -other.IsAlign(*this);
}

int tSegment::IsPointOnLine(tPoint const& point) const
{
	tInt128 signedArea = (static_cast<tInt128>(point.x) - End(0).x) * 
		(static_cast<tInt128>(End(1).y) - point.y) - 
		(static_cast<tInt128>(point.y) - End(0).y) * 
		(static_cast<tInt128>(End(1).x) - point.x);
	if (signedArea == static_cast < tInt128>(0)) {
		return 0;
	}
	return (signedArea > static_cast < tInt128>(0)) ? -1 : 1;
}


bool Less(tSegment const& first, tSegment const& second)
{
	auto align = first.IsAlign(second);
	if (align == 0) {
		if (first.End(0).y != second.End(0).y)
			return (first.End(0).y < second.End(0).y);
		return (first.End(0).x < second.End(0).x);
	}
	return (align == 1);
}

void SortSegments(std::list<tSegment>& segments)
{
	for (auto& seg : segments) {
		if (seg.End(0).x > seg.End(1).x)
			seg.SwapEnds();
		else if ((seg.End(0).x == seg.End(1).x) && (seg.End(0).y > seg.End(1).y))
			seg.SwapEnds();
	}
	segments.sort(Less);
}
