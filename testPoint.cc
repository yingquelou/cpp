/* C++类与平面向量？矢量？位移？LI */
#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include "Point.h"
int main(void)
{
    /* 创建一个点或向量 */
    Point O;           //O是一个原点或一个零向量
    cout << O << endl; //以 (横坐标,纵坐标) 形式输出坐标/向量,此处是(0,0)
    Point A(1, 1);     //点A的坐标为(1, 1),也可以看作一个向量
    Point B(2, 1);     //B(2, 1)
    Point C(A);        //C(1, 1)
    Point E(A, 2, 3);  //E=(2*1,3*1)=(2,3)
    /* 运算符重载 */
    Point D = A;           //D(1, 1)
    Point F = Point(2, 1); //使用匿名对象赋值点A的坐标为(1, 1)
    F *= Point(3, 4);      //F=(2*3,1*4)=(6,4),当前向量的横坐标变为原来的P.x倍,纵坐标变为原来的P.y倍
    ++F;                   //F=(6+1,4)=(7,4),前置递增为横坐标递增
    F++;                   //F=(7,4+1)=(7,5),后置递增为纵坐标递增
    --F;                   //F=(7-1,5)=(6,5),前置递减为横坐标递减
    F--;                   //F=(6,5-1)=(6,4),后置递减为纵坐标递减
    Point G = -F;          //G=-(6,4)=(-6,-4),向量取反
    /*向量的合成与分解——合成
     *点/向量相加即对应坐标相加
     *向量的加法满足平行四边形法则和三角形法则 */
    Point H = Point(1, 0) + Point(0, 1);
    if (Point(1, 0) == Point(1, 0)) //点/向量相同,条件为真
        ;
    if (Point(1, 0) == Point(1, 1)) //点/向量相异,条件为假
        ;
    double G_x = G.Get_x();     //获取点G的横坐标:-6
    double G_y = G.Get_y();     //获取点G的纵坐标:-4
    double dot_product = A * B; //向量的数量积(内积、点积)
    /*  *从上面来看,Point既可以定义(创建)一个点，也可以定义(创建)一个向量
        *为了对以上两种情况有所区分,给Point起了一个别名VectorOfMath
        *于是用Point定义(创建)一个点,用VectorOfMath定义(创建)一个向量   */
    VectorOfMath AB = B - A;        //AB是从点A到点B的向量
    if (Point(1, 1) || Point(2, 2)) //向量平行(共线),条件为真
        ;
    if (Point(1, 3) || Point(2, 2)) //向量不平行(不共线),条件为假
        ;
    double AB_Mold = AB.VectorMold(); //返回向量的模
    return 0;
}