#pragma once
#include "Point.h"
/* 双曲线————椭圆类(焦点仅在横轴的)
*构造椭圆的常规方法：
*1. 长轴 短轴
*2. 焦点 定长
*3. 焦点 准线 离心率*/
class Oval
{
private:
    //焦点
    Point Focus;
    //定长
    double FixedLength;
    //准线
    Point AlignLine;
    //长轴
    double LongAxis;
    //短轴
    double Brachyaxis;
    //离心率
    double EllipseEccentricity;

public:
    //无参构造
    Oval();
    //拷贝构造
    Oval(const Oval &);
    //长轴 短轴——构造
    Oval(const double &, const double &);
    //焦点 定长——构造
    Oval(const Point &, const double &);
    // 焦点 准线 离心率——构造
    Oval(const Point &, const Point &, const double);
    ~Oval();
};
Oval::Oval()
{
    Focus = Point(0, 0);
    FixedLength = 0;
    AlignLine = Point(0, 0);
    EllipseEccentricity = 0;
    LongAxis = 0;
    Brachyaxis = 0;
}
Oval::Oval(const Oval &ano)
{
    *this = ano;
}
Oval::Oval(const double &Long, const double &Brach)
{
}
Oval::Oval(const Point &foc, const double &fixed)
{
    Focus = foc;
    FixedLength = fixed;
}
Oval::Oval(const Point &, const Point &, const double)
{
}