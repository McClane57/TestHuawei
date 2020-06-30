#include "tShadow.h"
#include <cassert>

void tLadder::xInit()
{
	if (xDiagonal.End(0).y > xDiagonal.End(1).y) {
		xDiagonal.SwapEnds();
	}
	xDirections = xDiagonal.Directions();
	assert(!(xDirections.first == xDirections.second) && "bad segment for ladder");
	xShadow = tShadow(xDirections.second, xDirections.first, xDiagonal.End(1));
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

int tShadow::ReturnLimit(tPoint const& point) const
{
	auto iter = xMap.lower_bound(point.Limit(xKeyDir));
	if (iter->first < point.Limit(xKeyDir))
		++iter;
	assert(iter != xMap.end());
	return iter->second;
}
