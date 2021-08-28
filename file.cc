#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main(void)
{
    printf("%.20lf\n", M_PI);
    ofstream file;
    file.open(__FILE__, ios::app | ios::binary);
    for (size_t i = 0; i < 10; i++)
        file << "/*" << __LINE__ << " " << __FILE__ << " " << __DATE__ << " "
             << __TIME__ << "*/" << endl;
    file.close();
    return 0;
}
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:00*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
/*11 C:\Users\bfp\Desktop\cpp\file.cc Aug 28 2021 17:57:51*/
