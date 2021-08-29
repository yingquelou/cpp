#pragma once
#include "Point.h"
using namespace std;
//平面直线类
class Line
{
private:
    double A;
    double B;
    double C;

public: //构造与析构
        //无参构造，A与B均为零，这样的直线不存在
        //此时仅调用成员函数IsExist()有意义
    Line();
    //斜截式:y=kx+b(斜率必须存在),无法用于构造平行于纵轴的直线
    Line(const double &k, const double &b);
    //两点式
    // Line(const Point &A, const Point &B);
    //点方向式
    // Line(const Point &A, const Point &B);
    //点法向式
    // Line(const Point &A, const Point &B);
    //一般式：ax+by+c=0(a与b不能都为零)，可以用于构造任意平面直线
    Line(const double &a, const double &b, const double &c);
    // ~Line();

public:
    //判断直线的存在性
    bool IsLineExist();
};

/* Line::Line()
{
} */

/* Line::~Line()
{
} */
