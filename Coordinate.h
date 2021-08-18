#pragma once
#include <iostream>
using namespace std;
class Coordinate
{
private:
    double x;
    double y;

public:
    Coordinate();
    Coordinate(const Coordinate &point);
    Coordinate(const double &, const double &);
    ~Coordinate();
};
Coordinate::Coordinate() : x(0), y(0) {}
Coordinate::Coordinate(const double &a, const double &b) : x(a), y(b) {}
Coordinate::Coordinate(const Coordinate &point) : x(point.x), y(point.y) {}
Coordinate::~Coordinate() {}