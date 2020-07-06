#include "tShadow.h"
#include <cassert>

void tLadder::xInit()
{
	if (xDiagonal.End(0).x > xDiagonal.End(1).x) {
		xDiagonal.SwapEnds();
	}
	xDirections = xDiagonal.Directions();
	assert(!(xDirections.first == xDirections.second) && "bad segment for ladder");
	xShadow = tShadow(Next(xDirections.second), Next(xDirections.first,1,false), xDiagonal.End(1));
}

void tLadder::AddObstacle(tOctet const& octet)
{
	tOctet inflated(octet);
	inflated.Inflate(xDiagonal.Width());
	auto real_obst = xFrame.Intersect(&inflated);
	if (real_obst == nullptr)
		return;
	if (xDiagonal.IsPointOnLine(real_obst->Vertex(E)) > 0)
		return;
	xShadow.AddObstacle(*real_obst);
}

tPoint tLadder::LastPointUnderDiagonal(tPoint const& start) const
{
	auto lim1_start = xDiagonal.End(0).Limit(Next(xDirections.first,1,false));
	auto lim1_finish = xDiagonal.End(1).Limit(Next(xDirections.first, 1, false));
	auto lim1_current = start.Limit(Next(xDirections.first, 1, false));

	auto lim2_start = xDiagonal.End(0).Limit(xDirections.second);
	auto lim2_finish = xDiagonal.End(1).Limit(xDirections.second);
	auto lim2_current = start.Limit(xDirections.second);

	auto lim2_new = static_cast<int>
		(std::floor(static_cast<long long>(lim2_finish - lim2_start) * (lim1_current - lim1_start) /
			(lim1_finish - lim1_start)));
	int shift = lim2_new - lim2_current;
	if (xDirections.second % 2) {
		if (shift % 2) {
			shift -= 1;
		}
	}
	tSegment segment(start, xDirections.second, shift);

	return segment.End(1);
}

tTrajectory tLadder::MakeLadder() const
{
	tTrajectory result(xDiagonal.End(0));
	while (!(result.Trajectory().back() == xDiagonal.End(1))) {
		result.AddPoint(xShadow.NextPoint(result.Trajectory().back()));
		result.AddPoint(LastPointUnderDiagonal(result.Trajectory().back()));
	}
	assert(result.Trajectory().front() == xDiagonal.End(0) && "bad trajectory start");
	assert(result.Trajectory().back() == xDiagonal.End(1) && "bad trajectory end");
	assert(result.IsRegular() && "bad trajectory");
	return result;
}


std::map<int,int>::const_iterator tShadow::xIfAddable(int key, int val) const
{
	auto iter = xMap.lower_bound(key);
	if (iter->first < key) {
		++iter;
	}
	if (iter->second > val)
		return iter;
	else
		return xMap.end();
}

void tShadow::xRemoveExtra(int inserted_key)
{
	std::map<int,int>::iterator iter = xMap.find(inserted_key);
	auto val = iter->second;
	if (iter == xMap.begin())
		return;
	while (iter != xMap.begin()) {
		auto jter = iter;
		--jter;
		if (jter->second >= val) {
			xMap.erase(jter);
		}
		else
			return;
	}
	return;
}

tShadow::tShadow(eRegDir key, eRegDir value, tPoint const& end): xValueDir(value), xKeyDir(key)
{
	assert(xKeyDir == Next(xValueDir,3) && "bad directions for the shadow");
	xMap[INT32_MAX] = end.Limit(value);
}

tShadow tShadow::operator=(tShadow const& shadow)
{
	xMap = shadow.xMap;
	xValueDir = shadow.xValueDir;
	xKeyDir = shadow.xKeyDir;
	return *this;
}

bool tShadow::AddObstacle(tOctet const& octet)
{
	assert(xMap.empty() == false && "bad shadow");
	auto val = octet.Limit(xValueDir);
	auto key = octet.Limit(xKeyDir);
	auto addable = xIfAddable(key, val);
	if (addable != xMap.end()) {
		auto iter = xMap.find(key);
		if (iter != xMap.end()) {
			xMap.erase(iter);
		}
		xMap[key] = val;
		assert(xMap.find(key) != xMap.end() && "bad insertion to the shadow");
		xRemoveExtra(key);
		return true;
	}
	return false;
}

tPoint tShadow::NextPoint(tPoint const& start) const
{
	auto newLimit = xReturnLimit(start);
	auto startLimit = start.Limit(xValueDir);
	auto diff = newLimit - startLimit;
	assert(diff > 0);
	if (xValueDir % 2 == 0) {
		diff *= 2;
	}
	tSegment segment(start, Next(xValueDir), diff);
	return segment.End(1);
}

int tShadow::xReturnLimit(tPoint const& point) const
{
	auto iter = xMap.lower_bound(point.Limit(xKeyDir));
	if (iter->first <= point.Limit(xKeyDir))
		++iter;
	assert(iter != xMap.end());
	return iter->second;
}
