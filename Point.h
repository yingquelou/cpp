#pragma once
#include <iostream>
using namespace std;
class Point
{
private:
    double x;
    double y;

public:
    //向量或者说点坐标拷贝
    Point operator=(const Point &b);
    //向量的数量积
    double operator*(const Point &b);
    ostream &operator<<(ostream &mo);

public:
    //无参构造
    Point();
    //拷贝构造
    Point(const Point &point);
    Point(const double &, const double &);
    ~Point();
};
Point::Point() : x(0), y(0) {}
Point::Point(const double &a, const double &b) : x(a), y(b) {}
Point::Point(const Point &point) : x(point.x), y(point.y) {}
Point::~Point() {}
double Point::operator*(const Point &b)
{
    return x * b.x + y * b.y;
}
ostream &Point::operator<<(ostream &mo)
{
    mo << "(" << x << "," << y << ")";
    return mo;
}
Point Point::operator=(const Point &b)
{
    x = b.x;
    y = b.y;
    return *this;
}