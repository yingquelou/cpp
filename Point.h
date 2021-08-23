#pragma once
#include <cmath>
using std::sqrt;
#include <iostream>
using namespace std;
// 平面向量/点类(坐标表示)
class Point
{ //友元

    //以 (横坐标,纵坐标) 形式输出坐标/向量,例如：(1,1)
    friend ostream &operator<<(ostream &cout, const Point &P);
    //向量的数乘(数前置版) 滑动向量的产生
    friend Point operator*(const double &b, const Point &P);

private: //成员变量
    double x;
    double y;

public: //构造与析构
    //无参构造
    Point();
    //拷贝构造
    Point(const Point &point);
    //有参构造1
    Point(const double &, const double &);
    //有参构造2
    Point(const double &, const double &, const Point &P);
    ~Point();

public: //重载运算符
    //向量/坐标拷贝
    Point &operator=(const Point &b);
    //向量的数量积(内积、点积)
    double operator*(const Point &b) const;
    //向量的数乘(数后置版) 滑动向量的产生
    Point operator*(const double &b) const;
    Point &operator*=(const Point &P);
    //向量的合成与分解——合成
    //点/向量相加即对应坐标相加
    //向量的加法满足平行四边形法则和三角形法则
    Point operator+(const Point &P) const;
    //点/向量相减即对应坐标相减
    //向量的减法满足三角形法则
    Point operator-(const Point &P) const;
    //前置递减为横坐标递减
    Point &operator--();
    //后置递减为纵坐标递减
    Point &operator--(int);
    //前置递增为横坐标递增
    Point &operator++();
    //后置递增为纵坐标递增
    Point &operator++(int);
    //判断点/向量相同
    bool operator==(const Point &P) const;
    //非零向量间判断平行
    bool operator||(const Point &P) const;

public: //成员函数
    //滑动向量
    Point Sliding(const double &, const double &, const Point &);
    //向量的模
    double VectorMold() const;
    //向量的合成与分解——分解
    //求当前向量在另一非零向量上的分量
    Point VectorComponent(const Point &P) const;
    //求当前向量在以P1和P2为基底的坐标系中的坐标表示
    Point CoordinateInBase(const Point &P1, const Point &P2) const;
};
using VectorOfMath = Point;
Point::Point() : x(0), y(0) {}
Point::Point(const double &a, const double &b) : x(a), y(b) {}
Point::Point(const Point &P) : x(P.x), y(P.y) {}
Point::~Point() {}
Point::Point(const double &a, const double &b, const Point &P)
{
    x = a * P.x;
    y = b * P.y;
}
Point Point::operator*(const double &b) const
{
    return Point(b * x, b * y);
}
Point operator*(const double &b, const Point &P)
{
    return Point(b * P.x, b * P.y);
}
Point Point::CoordinateInBase(const Point &P1, const Point &P2) const
{
    Point P1_Component = this->VectorComponent(P1);
    Point P2_Component = this->VectorComponent(P2);
    double tx = P1_Component.VectorMold() / P1.VectorMold();
    double ty = P2_Component.VectorMold() / P2.VectorMold();
    if (*this * P1 < 0)
        tx = -tx;
    if (*this * P2 < 0)
        ty = -ty;
    return Point(tx, ty);
}
Point Point::VectorComponent(const Point &P) const
{
    double P_Mold = P.VectorMold();
    if (!P_Mold)
    //此时P为零向量，参数非法
    {
        cerr << "errer" << endl;
        exit(EXIT_FAILURE);
    }
    double proportion = ((*this * P) / P_Mold) / P_Mold;
    return proportion * P;
}
double Point::VectorMold() const
{
    return sqrt(x * x + y * y);
}
Point Point::Sliding(const double &a, const double &b, const Point &P)
{
    return Point(a * P.x, b * P.y);
}
bool Point::operator==(const Point &P) const
{
    if (x == P.x && y == P.y)
        return true;
    return false;
}
bool Point::operator||(const Point &P) const
{
    if (((*this == Point(0, 0)) + (P == Point(0, 0)) == 0) &&
        x * P.y == y * P.x)
        return true;
    return false;
}
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
    ++x;
    return *this;
}
Point &Point::operator++(int)
{
    ++y;
    return *this;
}
Point &Point::operator--()
{
    --x;
    return *this;
}
Point &Point::operator--(int)
{
    --y;
    return *this;
}
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