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
    Point O;                    //O是一个原点或一个零向量
    cout << O << endl;          //以 (横坐标,纵坐标) 形式输出坐标/向量,此处是(0,0)
    Point A(1, 1);              //点A的坐标为(1, 1),也可以看作一个向量
    Point B(2, 1);              //B(2, 1)
    Point C(A);                 //C(1, 1)
    Point E(A, 2, 3);           //E=(2*1,3*1)=(2,3)
    Point D = A;                //D(1, 1)
    Point F = Point(2, 1);      //使用匿名对象赋值点A的坐标为(1, 1)
    F *= Point(3, 4);           //F=(2*3,1*4)=(6,4)
    ++F;                        //F=(6+1,4)=(7,4)
    F++;                        //F=(7,4+1)=(7,5)
    --F;                        //F=(7-1,5)=(6,5)
    F--;                        //F=(6,5-1)=(6,4)
    double dot_product = A * B; //向量的数量积(内积、点积)
    cout << D << endl;
    /*  *从上面来看,Point既可以定义(创建)一个点，也可以定义(创建)一个向量
        *为了对以上两种情况有所区分,给Point起了一个别名VectorOfMath
        *于是用Point定义(创建)一个点,用VectorOfMath定义(创建)一个向量   */
    VectorOfMath AB = B - A; //AB是从点A到点B的向量
    cout << AB << endl;
    cout << AB++ << endl;

    return 0;
}