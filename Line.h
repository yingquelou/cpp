#pragma once
#include "Point.h"
#include <cstdbool>
using namespace std;
//平面直线类
class Line
{
private:
    double A;
    double B;
    double C;

public: //构造与析构
        //无参构造，A与B均为零,这样的直线不存在,无意义
        //此时仅调用成员函数IsExist()有意义
    Line();
    //拷贝构造
    Line(const Line &L);
    //有参构造之斜截式:y=kx+b(斜率必须存在),因此无法用于构造平行于纵轴的直线
    Line(const double &k, const double &b);
    //有参构造之两点式:相异的两点确定一条平面直线
    //当两点相同/重合时,此构造与无参构造效果相同
    Line(const Point &P1, const Point &P2);
    //点方向式  有一个占位参数
    Line(const Point &P, const VectorOfMath &DV, int);
    //点法向式  有两个占位参数
    Line(const Point &P, const VectorOfMath &NV, int, int);
    //一般式：ax+by+c=0(a与b不能都为零)，可以用于构造任意平面直线
    Line(const double &a, const double &b, const double &c);
    // ~Line();
public: //运算符重载
        //拷贝赋值
    Line &operator=(const Line &L);
    //判断平行
    bool operator||(const Line &L) const;

public: //成员函数
    /*  判断直线的存在性:
        当该类对象(该直线)对应的一般式ax+by+c=0中的a和b同时为零时,
        函数返回False,表明该直线(this所指的类对象所对应的)此时不存在或无意义 */
    bool IsLineExist(void);
    //求直线上横坐标为x的点的坐标,以参数P带回结果,成功返回true
    //当相应的直线平行于纵轴时,求解无意义,返回False
    bool KnowX(const double &x, Point &P) const;
    //求直线上纵坐标为y的点的坐标,以参数P带回结果,成功返回true
    //当相应的直线平行于横轴时,求解无意义,返回False
    bool KnowY(const double &y, Point &P) const;
    //求不平行的两直线的交点,以参数P带回结果,成功返回true
    //当两直线平行或重合时,求解无意义,返回False
    bool CrossoverPoint(const Line &L, Point &P) const;
    //返回平面直线的方向向量
    //当返回的是(0,0)时,表明该直线(相应this所指的类对象)此时不存在或无意义,
    //即它调用IsLineExist会返回False
    VectorOfMath DirectionVector(void) const;
    //返回平面直线的法向量
    //当返回的是(0,0)时,表明该直线(相应this所指的类对象)此时不存在或无意义,
    //即它调用IsLineExist会返回False
    VectorOfMath NormalVector(void) const;
    //返回两平面直线的夹角(指锐角夹角)
    double AngleOfLine(const Line &L) const;
};