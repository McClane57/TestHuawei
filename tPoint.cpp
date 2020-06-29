#include "tPoint.h"

bool operator==(const tPoint& left, const tPoint& right)
{
    return left.x == right.x && left.y == right.y;
}

tPoint tPoint::operator=(const tPoint& right)
{
    if (this == &right) {
        return *this;
    }
    this->x = right.x;
    this->y = right.y;
    return *this;
}

int tPoint::Limit(eRegDir dir) const
{
    switch (dir){
    case E:
        return x;
    case NE:
        return x + y;
    case N:
        return y;
    case NW:
        return y - x;
    case W:
        return -x;
    case SW:
        return -y - x;
    case S:
        return -y;
    case SE:
        return x - y;
    default:
        return 0;
    }
}

const tPoint operator+(const tPoint& left, const tPoint& right)
{
    return tPoint(left.x + right.x, left.y + right.y);
}

const tPoint operator-(const tPoint& left, const tPoint& right)
{
    return tPoint(left.x - right.x, left.y - right.y);
}

const tPoint operator*(const tPoint& left, int right)
{
    return tPoint(left.x * right, left.y * right);
}

std::ostream& operator<<(std::ostream& os, const tPoint& point)
{
    os << "x = " << point.x << ", y = " << point.y << "\n";
    return os;
}