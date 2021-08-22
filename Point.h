#pragma once
#include <iostream>
using namespace std;
// 向量/点类(数学)
class Point
{
    friend ostream &operator<<(ostream &cout, const Point &P);

private:
    double x;
    double y;

public:
    //向量/坐标拷贝
    Point &operator=(const Point &b);
    //向量的数量积
    double operator*(const Point &b) const;
    //点相加即对应坐标相加
    Point operator+(const Point &P) const;
    //点相减即对应坐标相减
    Point operator-(const Point &P) const;
    //前置递减为横坐标递减
    Point &operator--();
    //后置递减为纵坐标递减
    Point &operator--(int);
    //前置递增为横坐标递增
    Point &operator++();
    //后置递增为纵坐标递增
    Point &operator++(int);

public:
    //无参构造
    Point();
    //拷贝构造
    Point(const Point &point);
    Point(const double &, const double &);
    ~Point();
};
Point Point::operator+(const Point &P) const
{
    return Point(x + P.x, y + P.y);
}
Point Point::operator-(const Point &P) const
{
    return Point(x - P.x, y - P.y);
}
Point &Point::operator++()
{
    x++;
    return *this;
}
Point &Point::operator++(int)
{
    y++;
    return *this;
}
Point &Point::operator--()
{
    x--;
    return *this;
}
Point &Point::operator--(int)
{
    y--;
    return *this;
}
Point::Point() : x(0), y(0) {}
Point::Point(const double &a, const double &b) : x(a), y(b) {}
Point::Point(const Point &point) : x(point.x), y(point.y) {}
Point::~Point() {}
double Point::operator*(const Point &b) const
{
    return x * b.x + y * b.y;
}
ostream &operator<<(ostream &cout, const Point &P)
{
    cout << "(" << P.x << "," << P.y << ")";
    return cout;
}
Point &Point::operator=(const Point &b)
{
    x = b.x;
    y = b.y;
    return *this;
}