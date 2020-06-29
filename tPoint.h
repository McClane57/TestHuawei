#pragma once
#include <iostream>
#include "limits.h"


struct tPoint
{
    int x, y;
    tPoint() : x(0), y(0) {}
    tPoint(int X, int Y) : x(X), y(Y) {}
    tPoint(const tPoint& point) : x(point.x), y(point.y) {}

    tPoint operator=(const tPoint& right);
    friend bool operator==(const tPoint& left, const tPoint& right);
    friend const tPoint operator+(const tPoint& left, const tPoint& right);
    friend const tPoint operator-(const tPoint& left, const tPoint& right);
    friend const tPoint operator*(const tPoint& left, int right);
    int Limit(eRegDir dir) const;
};

std::ostream& operator<<(std::ostream& os, const tPoint& point);