#include <stdexcept>
using std::runtime_error;
#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include <cstring>
using std::string;
#include <vector>
using std::vector;
static int Serch(const vector<string> &s, const string &t);
int main(void)
{
    //定义串容器
    vector<string> s;
    //定义计数容器
    vector<unsigned> count;
    string t;
    int pos;
    /* for (decltype(run.size()) i = 0; i < 10; i++)
        run.push_back(i); */
    while (cin >> t)
    {
        if (s.empty() || (pos = Serch(s, t)) < 0)
        {
            if (!t.empty())
            {
                s.push_back(t);
                count.push_back(1);
            }
            else
            {
                cerr << "The t is empty.it's not decent!" << endl;
                return 0;
            }
        }
        else
            ++count[pos];
    }
    for (size_t i = 0; i < s.size(); i++)
        cout << s[i] << " is " << count[i] << "." << endl;
    return 0;
}
//在串容器中查找某串，找到了返回相应位置(以下标形式),
//参数不合法返回-1,找不到返回-2
static int Serch(const vector<string> &s, const string &t)
{
    if (s.empty() || t.empty())
        return -1; //参数不合法返回-1
    int pos = 0;
    for (const auto &i : s)
        if (i != t)
            ++pos;
        else
            break;
    if (pos >= s.size()) //找不到返回-2
        return -2;
    return pos;
}