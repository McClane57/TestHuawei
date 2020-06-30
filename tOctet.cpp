#include "tOctet.h"
#include "math.h"
#include <cassert>

tOctet& tOctet::Inflate(int difference)// if negative - deflate
{
	if (difference == 0) {
		return *this;
	}
	else {
		// semantic check that deflate is feasible
		if (!IsInflateble(difference))
		{
			assert(false && "Is not inflateble");
			return *this;
		}
		for (auto i = 0; i < 8; ++i) {
			xLimits[i] += (i%2 == 0)? difference: (difference > 0?
				static_cast<int>(2*std::floor(difference / std::sqrt(2) + 1)): -2 * static_cast<int>(std::floor(-difference / std::sqrt(2) + 1)));
		}
	}
	return *this;
}

void tOctet::Cure()
{
	for (auto i = 0; i < 4; ++i) {
		assert(Limit(eRegDir(i)) + Limit(eRegDir(i + 4)) >= 0 && "empty octet");
	}
	for (auto i = 1; i < 8; i+=2) {
		if (Limit(eRegDir(i)) > Limit(Next(eRegDir(i))) + Limit(Next(eRegDir(i), 1, false)))
			xLimits[i] = Limit(Next(eRegDir(i))) + Limit(Next(eRegDir(i), 1, false));
	}
	for (auto i =0; i < 8; i+=2)
	{
		int dol = Limit(Next(eRegDir(i),1,false)) - Limit(Next(eRegDir(i)));
		if (dol > 2 * Limit(Next(eRegDir(i),2, false)))
		{
			xLimits[i] = Limit(Next(eRegDir(i),2, false)) + Limit(Next(eRegDir(i)));
			xLimits[Next(eRegDir(i),1,false)] = Limit(Next(eRegDir(i),2, false)) + Limit(eRegDir(i));
		}
		else
		{
			if (dol + 2 * Limit(Next(eRegDir(i),2)) < 0)
			{
				xLimits[i] = Limit(Next(eRegDir(i),2)) + Limit(Next(eRegDir(i),1,false));
				xLimits[Next(eRegDir(i))] = Limit(Next(eRegDir(i),2)) + Limit(eRegDir(i));
			}
			else
			{
				int ml = Limit(Next(eRegDir(i), 1, false)) + Limit(Next(eRegDir(i)));
				if (2*xLimits[i] > ml)
					xLimits[i] = static_cast<int>(ml/2);
			}
		}
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
		assert(false && "Bad Dir");
		return;
	}
}

tOctet tOctet::operator=(tOctet const& second)
{
	if (second == *this) 
		return *this;
	else {
		for (auto i = 0; i < 8; ++i) {
			xLimits[i] = second.Limit(eRegDir(i));
		}
	}
	return *this;
}

void tOctet::CoverPoint(const tPoint& point)
{
	for (auto i = 0; i < 8; ++i) {
		xLimits[i] = point.Limit(eRegDir(i)) > xLimits[i] ? point.Limit(eRegDir(i)) : xLimits[i];
	}
	Cure();
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
	Cure();
	tPoint result;
	xNeighbourCommonPoint(dir, Next(dir), result);
	return result;
}

bool tOctet::IsInflateble(int difference) const {
    if (difference >= 0) return true;
	for (auto i = 0; i < 4; ++i) {
		bool sides_inflatable;
		if (i % 2 == 0)
			sides_inflatable = Limit(eRegDir(i)) + Limit(OppositeDirection(eRegDir(i))) >= -2 * difference;
		else
			sides_inflatable = Limit(eRegDir(i)) + Limit(OppositeDirection(eRegDir(i))) >= 2 * std::floor(-difference / std::sqrt(2) + 1);
		if (!sides_inflatable)
			return false;
	}
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

bool tOctet::HasIntersection(const tOctet* second) const
{
	for (auto i = 0; i < 8; ++i) {
		if (this->Limit(eRegDir(i)) + second->Limit(OppositeDirection(eRegDir(i))) < 0)
			return false;
	}
	return true;
}

std::shared_ptr<tOctet> tOctet::Intersect(const tOctet* second) const
{
	bool intersect = HasIntersection(second);
	if (!intersect)
		return nullptr;
	std::array<int, 8> limits;
	for (auto i = 0; i < 8; ++i) {
		limits[i] = this->Limit(eRegDir(i)) > second->Limit(eRegDir(i)) ? second->Limit(eRegDir(i)) : this->Limit(eRegDir(i));
	}
	std::shared_ptr<tOctet> intersection(new tOctet(limits));
	intersection->Cure();
	return intersection;
}

bool operator==(tOctet const& first, tOctet const& second)
{
	for (auto i = 0; i < 8; ++i) {
		if (first.Limit(eRegDir(i)) != second.Limit(eRegDir(i))) {
			return false;
		}
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const tOctet& octet)
{
	for (auto i = 0; i < 8; ++i) {
		os << int(octet.Limit(eRegDir(i))) << ", ";
	}
	os << "\n";
	return os;
}
