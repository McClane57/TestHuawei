#include "tSegment.h"
#include <cassert>

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