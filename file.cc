#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main(void)
{
    ofstream file;
    file.open(__FILE__, ios::app);
    for (size_t i = 0; i < 10; i++)
        file << "/*" << __LINE__ << " " << __FILE__ << " " << __DATE__ << " "
             << __TIME__ << "*/" << endl;
    file.close();
    return 0;
}
