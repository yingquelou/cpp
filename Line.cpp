#pragma once
#include "Line.h"
using namespace std;
Line::Line() : A(0), B(0), C(0) {}
Line::Line(const double &a, const double &b, const double &c) : A(a), B(b), C(c) {}
Line::Line(const Line &L)
{
    *this = L;
}
Line::Line(const double &k, const double &b)
{
    *this = Line(k, -1.0, b);
}
Line::Line(const Point &P, const VectorOfMath &DV, int)
{
    A = DV.Get_y();
    B = -DV.Get_x();
    C = -(A * P.Get_x() + B * P.Get_y());
}
Line::Line(const Point &P1, const Point &P2)
{
    VectorOfMath DV = P2 - P1; /* 方向向量 */
    if (DV == VectorOfMath(0.0, 0.0))
        *this = Line(0.0, 0.0, 0.0);
    else if (DV || VectorOfMath(0.0, 1.0))
        *this = Line(1.0, 0.0, -P1.Get_x());
    else
        *this = Line(P1, DV, 1);
}
Line::Line(const Point &P, const VectorOfMath &NV, int, int)
{
    VectorOfMath DV = {NV.Get_y(), -NV.Get_x()};
    *this = Line(P, DV, 1);
}
Line &Line::operator=(const Line &L)
{
    A = L.A;
    B = L.B;
    C = L.C;
    return *this;
}
bool Line::KnowX(const double &x, Point &P) const
{
    if (!B)
        return false;
    P = Point(x, -(A * x + C) / B);
    return true;
}
bool Line::KnowY(const double &y, Point &P) const
{
    if (!A)
        return false;
    P = Point(-(B * y + C) / A, y);
    return true;
}
bool Line::CrossoverPoint(const Line &L, Point &P) const
{
    if (*this || L)
        return false;
    double Xthis = B * L.C / (L.A * B - A * L.B),
           XP = L.B * C / (A * L.B - L.A * B);
    Point Pthis, PP;
    KnowX(XP, PP);
    L.KnowX(Xthis, Pthis);
    P = Pthis + PP;
    return true;
}
bool Line::operator||(const Line &L) const
{
    if (DirectionVector() || L.DirectionVector())
        return true;
    return false;
}
double Line::AngleOfLine(const Line &L) const
{
    double tmp = DirectionVector().AngleOfVector(L.DirectionVector());
    return (tmp > (M_PI / 2)) ? (M_PI - tmp) : tmp;
}
VectorOfMath Line::DirectionVector(void) const
{
    return VectorOfMath(-B, A);
}
VectorOfMath Line::NormalVector(void) const
{
    return VectorOfMath(A, B);
}
bool Line::IsLineExist(void)
{
    if (!A && !B)
        return false;
    return true;
}