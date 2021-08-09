#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <vector>
using std::vector;
int main(void)
{
    vector<int> run;
    for (decltype(run.size()) i = 0; i < 10; i++)
        run.push_back(i);
    for (auto a = run.begin(); a != run.end(); a++)
        *a *= 2;
    auto b = run.cbegin(),
         e = run.cend();
    auto x = e - b;
    cout << x << endl;
    run.end();
    return 0;
}