#include <stdio.h>
static struct
{
    //加法
    double (*Sum)(const double, const double);
    double (*Sub)(const double, const double);
    double (*Div)(const double, const double);
    double (*Mul)(const double, const double);

} Test;
static double mul(const double x, const double y)
{
    return x * y;
}
static double sum(const double x, const double y)
{
    return x + y;
}
static double div(const double x, const double y)
{
    if (!y)
        fprintf(stderr, "y can't be zero!\n");
    return x / y;
}
static double sub(const double x, const double y)
{
    return x - y;
}
void init(void)
{
    Test.Sum = sum;
    Test.Sub = sub;
    Test.Div = div;
    Test.Mul = mul;
}