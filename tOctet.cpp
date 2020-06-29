#include "tOctet.h"
#include "math.h"

tOctet& tOctet::Inflate(int difference)// if negative - deflate
{
	if (difference == 0) {
		return *this;
	}
	else {
		// semantic check that deflate is feasible
		if (!IsInflateble(difference))
		{
			std::cout << "Is not inflateble\n";
			return *this;
		}
		for (auto i = 0; i < 8; ++i) {
			xLimits[i] += (i%2 == 0)? difference: (difference > 0?
				2*std::floor(difference / std::sqrt(2) + 1): -2 * std::floor(-difference / std::sqrt(2) + 1));
		}
	}
	return *this;
}

void tOctet::xCure()
{
	for (auto i = 1; i < 8; i+=2) {
		auto neigbour_sum = xLimits[Next(eRegDir(i))] + xLimits[Next(eRegDir(i), 1, false)];
		xLimits[i] = xLimits[i] > neigbour_sum ? neigbour_sum : xLimits[i];
	}
}

void tOctet::xNeighbourCommonPoint(eRegDir dir1, eRegDir dir2, tPoint& result)
{
	if (dir2 != Next(dir1)) {
		auto tmp = dir1;
		dir1 = dir2;
		dir2 = tmp;
	}
	switch (dir1) {
	case E:
		result = tPoint(xLimits[dir1], xLimits[dir2] - xLimits[dir1]);
		return;
	case NE:
		result = tPoint(xLimits[dir1] - xLimits[dir2], xLimits[dir2]);
		return;
	case N:
		result = tPoint(xLimits[dir1] - xLimits[dir2], xLimits[dir1]);
		return;
	case NW:
		result = tPoint(-xLimits[dir2], xLimits[dir1] - xLimits[dir2]);
		return;
	case W:
		result = tPoint(-xLimits[dir1], xLimits[dir1] - xLimits[dir2]);
		return;
	case SW:
		result = tPoint(xLimits[dir2] - xLimits[dir1], -xLimits[dir2]);
		return;
	case S:
		result = tPoint(xLimits[dir2] - xLimits[dir1], -xLimits[dir1]);
		return;
	case SE:
		result = tPoint(xLimits[dir2], xLimits[dir2] - xLimits[dir1]);
		return;
	default:
		return;
	}
}

void tOctet::CoverPoint(const tPoint& point)
{
	for (auto i = 0; i < 8; ++i) {
		xLimits[i] = point.Limit(eRegDir(i)) > xLimits[i] ? point.Limit(eRegDir(i)) : xLimits[i];
	}
	xCure();
}

int tOctet::IsInside(const tPoint& point) const
{
	bool border = false;
	for (auto i = 0; i < 8; ++i) {
		if (point.Limit(eRegDir(i)) > xLimits[i]) {
			return 0;
		}
		else if (point.Limit(eRegDir(i)) == xLimits[i]){
			border = true;
		}
	}
	return border ? 1 : 2;
}

bool tOctet::IsFull() const
{
	for (auto i = 0; i < 8; ++i) {
		if (HasEdge(eRegDir(i)) == false) return false;
	}
	return true;
}

bool tOctet::HasEdge(eRegDir dir) const
{
	auto neigbour_sum = xLimits[Next(dir)] + xLimits[Next(dir, 1, false)];
	int current = (dir % 2 == 0) ? 2 * xLimits[dir] : xLimits[dir];
	return (current < neigbour_sum);
}

bool tOctet::CommonPoint(eRegDir dir1, eRegDir dir2, tPoint& result)
{
	if (NeighbourDirections(dir1, dir2)) {
		xNeighbourCommonPoint(dir1, dir2, result);
		return true;
	}
	return false;
}

tPoint tOctet::Vertex(eRegDir dir)
{
	xCure();
	tPoint result;
	xNeighbourCommonPoint(dir, Next(dir), result);
	return result;
}

bool tOctet::IsInflateble(int difference) const {
/*	if (difference >= 0) return true;
	for (auto i = 0; i < 4; ++i) {
		bool sides_inflatable = i % 2 == 0 ? Limit(eRegDir(i)) - Limit(Opposite(eRegDir(i))) >= -2 * difference:

	}*/
	return true;
}

bool tOctet::IsPoint()
{
	for (auto i = 0; i < 4; ++i)
	{
		if (xLimits[i] != -xLimits[i + 4])
		{
			return false;
		}
	}
	return true;
}

bool tOctet::IsRegularSegment()
{
	if (IsPoint()) return false;
	for (auto i = 0; i < 4; ++i)
	{
		if (xLimits[i] == -xLimits[i + 4])
		{
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const tOctet& octet)
{
	for (auto i = 0; i < 8; ++i) {
		os << int(octet.Limit(eRegDir(i))) << ", ";
	}
	os << "\n";
	return os;
}
